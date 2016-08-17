#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLShader>
#include <QImage>
#include <QFileDialog>
#include <iostream>
#include <QtOpenGL>

#include "EasyGL/EasyScene.h"

class OpenGLWidget : public QOpenGLWidget , protected QOpenGLFunctions
{
  Q_OBJECT

public:
  explicit OpenGLWidget(QWidget *parent = 0);
  ~OpenGLWidget();

  void paintGL();
  void resizeGL(int w, int h);
  void initializeGL();
  void updateTexture();

protected:
  void openSerialPort();
  void readSerialData();
  void parseData(QByteArray & rawData);

private:
  QStringList materialNames;
  EasyScene *scene;
  QVector <EasyMaterial*> materials;
  EasyGeometry *square;
  EasyMesh *squareMesh;
  QTimer *timer;

public slots:
  void updateScene();
};

#endif // OPENGLWIDGET_H
