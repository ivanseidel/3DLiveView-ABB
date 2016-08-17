#ifndef EASYMESH_H
#define EASYMESH_H

#include <QtOpenGL>
#include <QOpenGLFunctions>

#include "EasyScene.h"
#include "EasyGeometry.h"
#include "EasyMaterial.h"

class EasyScene;
class EasyMesh {
public:
  EasyMesh(EasyGeometry* geometry = NULL, EasyMaterial* material = NULL);

  void draw(EasyScene* scene, EasyMesh* parent);
  void computeModels(EasyMesh *parent);

  void setMaterial(EasyMaterial* material);

  void addChild(EasyMesh* child);

  bool visible;

  QVector3D position;
  QVector3D rotation;
  QVector3D scale;

  QMatrix4x4 model;
protected:
  QLinkedList<EasyMesh*> childs;

  EasyGeometry* geometry;
  EasyMaterial* material;
};

#endif
