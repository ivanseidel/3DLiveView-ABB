#ifndef EASYSCENE_H
#define EASYSCENE_H

#include <QOpenGLWidget>

#include "EasyCamera.h"
#include "EasyLight.h"

#include "EasyMesh.h"

class EasyMesh;

class EasyScene {
public:
    EasyScene(QOpenGLWidget *widget);

    void render(EasyMesh *root);

    EasyLight *light;
    EasyCamera *camera;
private:
    QOpenGLWidget *widget;
};

#endif // EASYSCENE_H
