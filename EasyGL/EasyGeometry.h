#ifndef EASYGEOMETRY_H
#define EASYGEOMETRY_H

#include <QtOpenGL>
#include <QOpenGLFunctions>

class EasyGeometry {
public:
  bool needsUpdate;
  EasyGeometry();

  void bind();
  void draw();
  void release();

  void loadFromOFF(const QString &file);
  void loadFromSTL(const QString &file, float normalizeScale = 1);

  void setTexture(QImage* img);

  QOpenGLVertexArrayObject *vao;
  QOpenGLBuffer* vboVertices;
  QOpenGLBuffer* vboIndices;
  QOpenGLBuffer* vboNormals;
  QOpenGLBuffer* vboMappings;

  QVector3D offset;
  double scale;

protected:
  QVector4D* vertices;
  QVector3D* normals;
  QVector2D* mappings;
  unsigned int* indices;

  unsigned int numVertices;
  unsigned int numFaces;

  void createVBOs();
  void destroyVBOs();
};

#endif
