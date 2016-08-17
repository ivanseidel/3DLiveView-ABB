#include "EasyScene.h"
#include "EasyMesh.h"

EasyScene::EasyScene(QOpenGLWidget *widget){
  this->widget = widget;
}

void EasyScene::render(EasyMesh *root){
    root->draw(this, NULL);
}
