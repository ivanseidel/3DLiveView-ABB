#include "EasyCamera.h"
#include "EasyCameraTransition.h"

EasyCameraTransition::EasyCameraTransition(EasyCamera *camFrom, EasyCamera *camTo, float _duration) : EasyCamera()
{
    from = camFrom;
    to = camTo;
    progress = 0.0;
    completed = false;
    duration = _duration;

    animate(0.0);
}

void EasyCameraTransition::gotoCamera(EasyCamera *_to, float _duration){
    from = to;
    to = _to;
    duration = _duration;
    progress = 0.0;
    completed = false;
}

float EasyCameraTransition::easeInOutQuad(float t, float b, float c, float d) {
    t /= d/2;
    if (t < 1) return c/2*t*t + b;
    t--;
    return -c/2 * (t*(t-2) - 1) + b;
}

void EasyCameraTransition::animate(float dt){
    progress += dt / duration;
    if(progress >= 1.0){
        progress = 1.0;

        if(completed){
            // Follow to camera
            to->update();
            projectionMatrix = to->projectionMatrix;
            viewMatrix = to->viewMatrix;
            center = to->center;
            up = to->up;
            return;
        }else
            completed = true;
    }

    float mappedProgress = easeInOutQuad(progress, 0, 1, 1);
    qDebug() << "Progress:" << progress << mappedProgress << " OK:" << completed;

    if(!to && !from){
        //qDebug() << "!to !from";
        // Does nothing
        projectionMatrix.setToIdentity();
        viewMatrix.setToIdentity();
    }else if(to && !from){
        //qDebug() << "to !from";
        to->update();
        projectionMatrix = to->projectionMatrix;
        viewMatrix = to->viewMatrix;
        center = to->center;
        up = to->up;
    }else if(!to && from){
        //qDebug() << "!to from";
        from->update();
        projectionMatrix = from->projectionMatrix;
        viewMatrix = from->viewMatrix;
        center = from->center;
        up = from->up;
    }else if(to && from){
        //qDebug() << "to from";
        // Compute matrixes
        projectionMatrix = to->projectionMatrix;
        position = (mappedProgress * to->position) + (1 - mappedProgress) * from->position;
        center = (mappedProgress * to->center) + (1 - mappedProgress) * from->center;
        up = (mappedProgress * to->up) + (1 - mappedProgress) * from->up;
        up.normalize();

        update();
    }
}
