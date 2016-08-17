#ifndef EASYMATERIAL_H
#define EASYMATERIAL_H

#include <QtOpenGL>
#include <QOpenGLFunctions>

class EasyMaterial {
public:
  EasyMaterial();
  ~EasyMaterial();

  void link();
  void bind();
  void release();

  void destroyVertexShader();
  void destroyFragmentShader();

  void setVertexShader(char* shader, bool readFile);
  void setFragmentShader(char* shader, bool readFile);

  void setTexture(QImage *img);

  QOpenGLShaderProgram* getProgram();

  QOpenGLBuffer* vboMappings;
  QOpenGLTexture* vboTexture;

  QVector4D ambient;
  QVector4D diffuse;
  QVector4D specular;
  double shininess;
  double textureOpacity;

protected:
  QOpenGLShaderProgram* shaderProgram;
  QOpenGLShader* fShader;
  QOpenGLShader* vShader;

  QVector2D* mappings;

};

#endif
