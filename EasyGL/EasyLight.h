#ifndef EASYLIGHT_H
#define EASYLIGHT_H

#include <QtOpenGL>

class EasyLight {
public:
    EasyLight();

    // RGB values
    QVector4D ambient;
    QVector4D diffuse;
    QVector4D specular;

    // Light position
    QVector4D position;
};

#endif // EASYLIGHT_H
