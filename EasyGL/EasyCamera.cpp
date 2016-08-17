#include "EasyCamera.h"


EasyCamera::EasyCamera(){
    position = QVector3D(0, 0, 1);
    center = QVector3D(0, 0, 0);
    up = QVector3D(0, 1, 0);

    projectionMatrix.setToIdentity();
    update();
}

void EasyCamera::lookAt(QVector3D _lookAt){
  center = _lookAt;
  update();
}

void EasyCamera::setPosition(QVector3D _position){
  position = _position;
  update();
}

void EasyCamera::update(){
  viewMatrix.setToIdentity();
  viewMatrix.lookAt(position, center, up);
}
