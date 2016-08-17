#ifndef EASYCAMERATRANSITION_H
#define EASYCAMERATRANSITION_H

#include "EasyCamera.h"

class EasyCameraTransition : public EasyCamera
{
public:
    EasyCameraTransition(EasyCamera *camFrom, EasyCamera *camTo, float _duration = 0.5);
    void gotoCamera(EasyCamera *to, float _duration);
    void animate(float dt);

    EasyCamera *from;
    EasyCamera *to;
    double progress;
    float duration;
    bool completed;

    float easeInOutQuad(float t, float b, float c, float d);
};

#endif // EASYCAMERATRANSITION_H
