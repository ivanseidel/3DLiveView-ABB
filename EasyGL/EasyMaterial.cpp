#include "EasyMaterial.h"

EasyMaterial::EasyMaterial(){
  qDebug() << "EasyMaterial created" << endl;

  shaderProgram = NULL;
  fShader = NULL;
  vShader = NULL;

  mappings = NULL;
  vboMappings = NULL;
  vboTexture = NULL;

  textureOpacity = 0.0;
}

EasyMaterial::~EasyMaterial(){
  qDebug() << "~EasyMaterial" << endl;

  destroyVertexShader();
  destroyFragmentShader();

  if(shaderProgram == NULL)
    return;

  shaderProgram->release();
  delete shaderProgram;
  shaderProgram = NULL;
}

void EasyMaterial::destroyVertexShader(){
  if(vShader == NULL)
    return;

  qDebug() << "EasyMaterial:destroyVertexShader" << endl;

  if(shaderProgram)
    shaderProgram->removeShader(vShader);

  delete vShader;
  vShader = NULL;
}


void EasyMaterial::setTexture(QImage *img){
  // qDebug() << "EasyGeometry:setTexture" << endl;
  if(vboTexture){
    delete vboTexture;
  }

  //Cria a Textura a partir da imagem
  vboTexture = new QOpenGLTexture(*img);
  vboTexture->bind();
  vboTexture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
  vboTexture->setWrapMode(QOpenGLTexture::DirectionS,QOpenGLTexture::MirroredRepeat);
  vboTexture->setWrapMode(QOpenGLTexture::DirectionT,QOpenGLTexture::MirroredRepeat);
  vboTexture->release();
  textureOpacity = 1.0;
}

void EasyMaterial::destroyFragmentShader(){
  if(fShader == NULL)
    return;

  qDebug() << "EasyMaterial:destryFragmentShader" << endl;

  if(shaderProgram)
    shaderProgram->removeShader(fShader);

  delete fShader;
  fShader = NULL;
}

void EasyMaterial::link(){
  qDebug() << "EasyMaterial:link" << endl;

  if(shaderProgram){
    shaderProgram->release();
    delete shaderProgram;
  }

  shaderProgram = new QOpenGLShaderProgram();

  if(vShader)
    shaderProgram->addShader(vShader);

  if(fShader)
    shaderProgram->addShader(fShader);

  if (!shaderProgram->link())
    qWarning() << shaderProgram->log() << endl;
}

void EasyMaterial::bind(){
  // qDebug() << "EasyMaterial:bind" << endl;
  if(vboTexture && vboMappings){
    vboTexture->bind();
    vboMappings->bind();
  }

  shaderProgram->bind();
}

void EasyMaterial::release(){
  // qDebug() << "EasyMaterial:release" << endl;
  shaderProgram->release();

  if(vboTexture && vboMappings){
    vboMappings->release();
    vboTexture->release();
  }

  // qWarning() << shaderProgram->log() << endl;
}

QOpenGLShaderProgram* EasyMaterial::getProgram(){
  return shaderProgram;
}


void EasyMaterial::setVertexShader(char* shader, bool readFile){
  qDebug() << "EasyMaterial:setVertexShader" << endl;
  destroyVertexShader();

  this->vShader = new QOpenGLShader(QOpenGLShader::Vertex);

  if(readFile)
    this->vShader->compileSourceFile(shader);
  else
    this->vShader->compileSourceCode(shader);

  if(!this->vShader->isCompiled())
    qWarning() << vShader->log();
  else
    qDebug() << "Compiled vertex shader\n";
}

void EasyMaterial::setFragmentShader(char* shader, bool readFile){
  qDebug() << "EasyMaterial:setFragmentShader" << endl;
  destroyFragmentShader();

  this->fShader = new QOpenGLShader(QOpenGLShader::Fragment);

  if(readFile){
    if(!this->fShader->compileSourceFile(shader))
      qWarning() << fShader->log();
  }else{
    if(!this->fShader->compileSourceCode(shader))
      qWarning() << fShader->log();
  }

  if(!this->fShader->isCompiled())
    qWarning() << fShader->log();
  else
    qDebug() << "Compiled fragment shader\n";
}
