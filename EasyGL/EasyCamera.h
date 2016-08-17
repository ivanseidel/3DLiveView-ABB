#ifndef EASYCAMERA_H
#define EASYCAMERA_H

#include <QtOpenGL>

class EasyCamera {
public:
    EasyCamera();

    void setPosition(QVector3D _position);
    void lookAt(QVector3D _lookAt);

    QMatrix4x4 projectionMatrix;
    QMatrix4x4 viewMatrix;

    QVector3D up;
    QVector3D center;
    QVector3D position;

    void update();
protected:
};

#endif // EASYCAMERA_H
