#include "BasicMaterial.h"

BasicMaterial* BasicMaterial::_singletonMaterial = NULL;

BasicMaterial::BasicMaterial(): EasyMaterial()
{
    ambient = QVector4D (.2, .2, .2, 1.0);
    diffuse = QVector4D (1.0, 1.0, 1.0, 1.0);
    specular = QVector4D (1, 1, 1, 1.0);
    shininess = 100.0;

    if(_singletonMaterial != NULL){
        this->shaderProgram = _singletonMaterial->shaderProgram;
        this->fShader = _singletonMaterial->fShader;
        this->vShader = _singletonMaterial->vShader;
        return;
    }

    qDebug() << "* Compiling Shaders...";
    this->setFragmentShader(":/shaders/advanced_fshader.glsl", true);
    this->setVertexShader(":/shaders/advanced_vshader.glsl", true);
    this->link();

    _singletonMaterial = this;
}
