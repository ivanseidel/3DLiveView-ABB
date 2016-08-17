#include "EasyLight.h"

EasyLight::EasyLight() {
    diffuse = QVector4D(1, 1, 1, 0);
    ambient = QVector4D(1, 1, 1, 0);
    specular = QVector4D(1, 1, 1, 0);

    position = QVector4D(0, 0, 1, 0);
}
