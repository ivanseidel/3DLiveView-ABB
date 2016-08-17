#include <QtOpenGL>
#include <QOpenGLFunctions>

#include "EasyMesh.h"
#include "EasyGeometry.h"
#include "EasyMaterial.h"
#include "EasyScene.h"

EasyMesh::EasyMesh(EasyGeometry* geometry, EasyMaterial* material){
  qDebug() << "EasyMesh:created" << endl;
  this->geometry = geometry;
  this->material = material;

  this->model.setToIdentity();

  this->scale = QVector3D(1,1,1);

  visible = true;
}

void EasyMesh::setMaterial(EasyMaterial* material){
  this->material = material;
}

void EasyMesh::addChild(EasyMesh *child){
    this->childs.append(child);
}

void EasyMesh::computeModels(EasyMesh *parent){
    // Reset model
    if(parent)
      model = parent->model;
    else
      model.setToIdentity();

    // Apply father transformation

    // Mesh specific
    model.scale(scale);
    model.translate(position);
    model.rotate(rotation.z(), 0, 0, 1);
    model.rotate(rotation.y(), 0, 1, 0);
    model.rotate(rotation.x(), 1, 0, 0);

    // Draw it's childs
    QLinkedList<EasyMesh*>::iterator i = childs.begin();
    while (i != childs.end()) {
        EasyMesh *child = *i;
        child->computeModels(this);
        ++i;
    }
}

void EasyMesh::draw(EasyScene* scene, EasyMesh *parent){
  // qDebug() << "EasyMesh:draw" << endl;

  // Reset model
  if(parent)
    model = parent->model;
  else
    model.setToIdentity();

  // Apply father transformation

  // Mesh specific
  model.scale(scale);
  model.translate(position);
  model.rotate(rotation.z(), 0, 0, 1);
  model.rotate(rotation.y(), 0, 1, 0);
  model.rotate(rotation.x(), 1, 0, 0);

  // Draw this mesh object if set
  if(material && geometry && visible){
      material->bind();
      geometry->bind();

      geometry->vboVertices->bind();
      material->getProgram()->enableAttributeArray("vPosition");
      material->getProgram()->setAttributeBuffer("vPosition", GL_FLOAT, 0, 4, 0);
      geometry->vboVertices->release();

      geometry->vboNormals->bind();
      material->getProgram()->enableAttributeArray("vNormal");
      material->getProgram()->setAttributeBuffer("vNormal", GL_FLOAT, 0, 3, 0);
      geometry->vboNormals->release();

      if(material->vboMappings){
        material->vboMappings->bind();
        material->getProgram()->enableAttributeArray("vcoordText");
        material->getProgram()->setAttributeBuffer("vcoordText", GL_FLOAT, 0, 2, 0);
        //material->vboMappings->release();
      }else if(geometry->vboMappings){
        geometry->vboMappings->bind();
        material->getProgram()->enableAttributeArray("vcoordText");
        material->getProgram()->setAttributeBuffer("vcoordText", GL_FLOAT, 0, 2, 0);
        //geometry->vboMappings->release();
      }

      if(material->vboTexture){
        material->vboTexture->bind();
        material->getProgram()->setUniformValue("colorTexture", 0);
        //material->vboTexture->release();
      }

      // Geometry specific
      QMatrix4x4 tmpModel = model;
      tmpModel.scale(geometry->scale, geometry->scale, geometry->scale);
      tmpModel.translate(geometry->offset);

      // Get shader
      QOpenGLShaderProgram *shader = material->getProgram();

      // Model uniforms
      //shader->setUniformValue(");
      shader->setUniformValue("textureOpacity", static_cast <GLfloat >(material->textureOpacity));
      shader->setUniformValue("model", tmpModel);
      shader->setUniformValue("normalMatrix", tmpModel.normalMatrix());

      // Camera uniforms
      if(scene && scene->camera){
          shader->setUniformValue("view", scene->camera->viewMatrix);
          shader->setUniformValue("projection", scene->camera->projectionMatrix);
      }

      // Light uniforms
      if(scene && scene->light) {
          shader->setUniformValue("lightPosition", scene->light->position);
          shader->setUniformValue("ambientProduct", scene->light->ambient * this->material->ambient);
          shader->setUniformValue("diffuseProduct", scene->light->diffuse * this->material->diffuse);
          shader->setUniformValue("specularProduct", scene->light->specular * this->material->specular);
          shader->setUniformValue("shininess", static_cast <GLfloat >(this->material->shininess));
      }

      // Draw object
      geometry->draw();

      geometry->release();
      material->release();
  }

  // Draw it's childs
  QLinkedList<EasyMesh*>::iterator i = childs.begin();
  while (i != childs.end()) {
      EasyMesh *child = *i;
      child->draw(scene, this);
      ++i;
  }
}
