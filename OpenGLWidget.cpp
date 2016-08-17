#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QMouseEvent>
#include <QDateTime>
#include <QTimer>

#include "MainWindow.h"
#include "OpenGLWidget.h"

#include "EasyGL/EasyCamera.h"
#include "EasyGL/EasyCameraTransition.h"
#include "EasyGL/EasyGeometry.h"
#include "EasyGL/EasyLight.h"
#include "EasyGL/EasyMaterial.h"
#include "EasyGL/EasyMesh.h"
#include "EasyGL/EasyScene.h"

#include "BasicMaterial.h"
#include "SquareGeometry.h"

#define SERIALPORT_NAME "cu.MonsterBT-DevB"
#define SERIALPORT_DATA_PARAMS    5

BasicMaterial *mOrange;

EasyMesh *armBase;
EasyMesh *armLink1;
EasyMesh *armLink2;
EasyMesh *armLink3;
EasyMesh *armLink4;

EasyCameraTransition *camera;
EasyCamera *cameraStatic;
EasyCamera *cameraArm;
EasyCamera *cameraUp;
EasyMesh *root;

EasyMaterial *mFloor;
SquareGeometry *gFloor;
EasyMesh *floorMesh;

EasyMaterial *mSky;
SquareGeometry *gBackground;
EasyMesh *background;

// Serial Port
QSerialPort serialPort;         // Novo objecto QSerialPort
QString serialPortName;         // Variavel que ira receber o caminho da porta serial
int serialPortBaudRate;         // Variavel que ira receber o baudrate da conexão

// Keeps values
float serialData[SERIALPORT_DATA_PARAMS];

//Construtor da classe
OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent) {

    scene = NULL;
    square = NULL;
    squareMesh = NULL;

    timer = new QTimer(this->parent());
    connect(timer, SIGNAL(timeout()), this, SLOT(updateScene()));
    timer->start(1000 / 30.0);

    // Setup Serial Port
    openSerialPort();
}

//Destrutor da classe
OpenGLWidget::~OpenGLWidget()
{
  if(serialPort.isOpen())
      serialPort.close();

  delete scene;
  // delete materials;
  delete square;
  delete squareMesh;
}

// Abre porta serial
void OpenGLWidget::openSerialPort(){
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        qDebug() << QObject::tr("Port: ") << info.portName() << endl
        << QObject::tr("Location: ") << info.systemLocation() << endl
        << QObject::tr("Description: ") << info.description() << endl
        << QObject::tr("Manufacturer: ") << info.manufacturer() << endl
        << QObject::tr("Serial number: ") << info.serialNumber() << endl
        << QObject::tr("Vendor Identifier: ") <<
            (info.hasVendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : QString()) << endl
        << QObject::tr("Product Identifier: ") <<
            (info.hasProductIdentifier() ? QString::number(info.productIdentifier(), 16) : QString()) << endl
        << QObject::tr("Busy: ") << (info.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) << endl;
    }

    serialPort.setPortName(SERIALPORT_NAME);

    serialPort.setBaudRate(QSerialPort::Baud115200, QSerialPort::AllDirections);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setFlowControl(QSerialPort::NoFlowControl);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);

    if (!serialPort.open(QIODevice::ReadWrite)) {
        qWarning() << QObject::tr("Falha ao abrir porta %1, erro: %2").arg(SERIALPORT_NAME).arg(serialPort.errorString()) << endl;
        // exit(1);
    }
}

// Atualiza dados vindos da porta serial
QByteArray inData;
void OpenGLWidget::readSerialData(){
    if(!serialPort.isOpen())
        return;

    // Reads data until no line available
    while(serialPort.canReadLine()){
        // Read and append data
        parseData(inData = serialPort.readLine());
    }

}

void OpenGLWidget::parseData(QByteArray & rawData){
    QString str = QString::fromLatin1(rawData);
    int spliters = std::count(str.begin(), str.end(), ':');
    if(spliters != SERIALPORT_DATA_PARAMS - 1){
        qWarning() << "Malformed data:" << str;
        return;
    }

    str.replace("\n", "");
    str.replace("\r", "");
    QStringList pieces = str.split(":");

    for(int i = 0; i < pieces.size(); i++){
        serialData[i] = pieces.at(i).toFloat();
    }

    qDebug() << "Parsed data. ID:" << serialData[0];
}

//Inicializa funções OpenGL
void OpenGLWidget::initializeGL()
{
  makeCurrent();
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);

  //Exibe informacoes sobre a versao do OpenGL e GLSL
  std :: cerr << " Version " << glGetString ( GL_VERSION ) <<" \n ";
  std :: cerr << " GLSL " << glGetString ( GL_SHADING_LANGUAGE_VERSION ) << endl << endl;

  // Create root mesh
  root = new EasyMesh();
  root->rotation.setX(-90);

  // Create light
  EasyLight *light = new EasyLight();
  light->position = QVector4D(0, 0, 4, 0);

  // Create basic cameras
  cameraArm = new EasyCamera();
  cameraArm->setPosition(QVector3D(0, 1, 0));
  cameraArm->lookAt(QVector3D(0, 1, 0));

  cameraUp = new EasyCamera();
  //cameraUp->up = QVector3D(1, 0, 0);
  cameraUp->setPosition(QVector3D(-0.5, 2, 0));
  cameraUp->lookAt(QVector3D(0, 0, 0));

  cameraStatic = new EasyCamera();
  cameraStatic->setPosition(QVector3D(0, 1, 2));
  cameraStatic->lookAt(QVector3D(0, 1, 0));

  // Create transition camera
  camera = new EasyCameraTransition(cameraUp,cameraStatic, 2);

  // Initialize scene
  scene = new EasyScene(this);
  scene->light = light;
  scene->camera = camera;

  // Create material
  BasicMaterial *mOrange = new BasicMaterial();
  mOrange->diffuse = QVector4D(228 / 255.0, 127 / 255.0, 40 / 255.0, 1.0);

  BasicMaterial *mDark = new BasicMaterial();
  mDark->diffuse = QVector4D(0.3, 0.3, 0.3, 1.0);
  mDark->shininess = 50;

  BasicMaterial *mRed = new BasicMaterial();
  mRed->diffuse = QVector4D(0.9, 0.1, 0.1, 1.0);

  // Create background mesh
  mSky = new EasyMaterial();
  mSky->setFragmentShader(":/shaders/background_fshader.glsl", true);
  mSky->setVertexShader(":/shaders/background_vshader.glsl", true);
  mSky->link();
  mSky->setTexture(new QImage(":/textures/background_mountains.jpg"));
  gBackground = new SquareGeometry();
  background = new EasyMesh(gBackground, mSky);

  // Create floor mesh
  mFloor = new BasicMaterial();
  mFloor->setTexture(new QImage(":/textures/floor.jpg"));
  mFloor->shininess = 5;
  mFloor->diffuse = QVector4D(0,0,0,0);
  mFloor->specular = QVector4D(0.3,0.3,0.3,0);
  mFloor->ambient = QVector4D(0,0,0,0);
  gFloor = new SquareGeometry();
  floorMesh = new EasyMesh(gFloor, mFloor);
  root->addChild(floorMesh);

  //EasyGeometry *camel = new EasyGeometry();
//  camel->loadFromOFF(":/models/camel.off");
  //camel->loadFromSTL(":/models/car.stl");
  //squareMesh = new EasyMesh(camel, camelMaterial);

  EasyGeometry *gArmBase = new EasyGeometry();
  gArmBase->loadFromSTL(":/models/IRB140_BASE.STL", 0.001);
  armBase = new EasyMesh(gArmBase, mOrange);
  armBase->position = QVector3D(0, 0, 0.02);
  root->addChild(armBase);

  EasyGeometry *gArmLink1 = new EasyGeometry();
  gArmLink1->loadFromSTL(":/models/IRB140_LINK1.STL", 0.001);
  armLink1 = new EasyMesh(gArmLink1, mOrange);
  armLink1->position = QVector3D(0, 0, 0.02);
  armBase->addChild(armLink1);

  EasyGeometry *gArmLink1Motors = new EasyGeometry();
  gArmLink1Motors->loadFromSTL(":/models/IRB140_LINK1_MOTORS.STL", 0.001);
  EasyMesh *armLink1Motors = new EasyMesh(gArmLink1Motors, mDark);
  armLink1Motors->position = QVector3D(0, 0, 0);
  armLink1->addChild(armLink1Motors);

  EasyGeometry *gArmLink2 = new EasyGeometry();
  gArmLink2->loadFromSTL(":/models/IRB140_LINK2.STL", 0.001);
  gArmLink2->offset = QVector3D(-70, 0, -352);
  armLink2 = new EasyMesh(gArmLink2, mOrange);
  armLink2->position = QVector3D(0.070, 0, 0.352);
  armLink1->addChild(armLink2);

  EasyGeometry *gArmLink3 = new EasyGeometry();
  gArmLink3->loadFromSTL(":/models/IRB140_LINK3.STL", 0.001);
  gArmLink3->offset = QVector3D(-70, -1, -712);
  armLink3 = new EasyMesh(gArmLink3, mOrange);
  armLink3->position = QVector3D(0.0, 0, 0.352);
  armLink2->addChild(armLink3);

  armLink3->visible = true;

  EasyGeometry *gArmLink4 = new EasyGeometry();
  gArmLink4->loadFromSTL(":/models/IRB140_LINK4.STL", 0.001);
  gArmLink4->offset = QVector3D(-288, 0, -712);
  armLink4 = new EasyMesh(gArmLink4, mRed);
  armLink4->position = QVector3D(0.218, 0, 0);
  armLink3->addChild(armLink4);
}


//Resize
void OpenGLWidget::resizeGL(int w, int h)
{
    qDebug() << " ! resizeGL" << w << h;
    cameraStatic->projectionMatrix.setToIdentity ();
    cameraStatic->projectionMatrix.perspective (
      60.0,
      static_cast <qreal>(w) / static_cast <qreal>(h),
      0.1,
      20.0);

    cameraArm->projectionMatrix.setToIdentity ();
    cameraArm->projectionMatrix.perspective (
      60.0,
      static_cast <qreal>(w) / static_cast <qreal>(h),
      0.1,
      20.0);

    cameraUp->projectionMatrix.setToIdentity ();
    cameraUp->projectionMatrix.perspective (
      60.0,
      static_cast <qreal>(w) / static_cast <qreal>(h),
      0.1,
      20.0);
}


//PaintGL
void OpenGLWidget::paintGL()
{
  makeCurrent();

  long startTime = QDateTime::currentMSecsSinceEpoch();

  // Read serial data
  readSerialData();

  // Reset screen
  glClear(GL_COLOR_BUFFER_BIT);

  // Draw Background and clear Depth buffer
  background->draw(scene, NULL);
  glClear(GL_DEPTH_BUFFER_BIT);

  // Render
  scene->render(root);

  int took = (QDateTime::currentMSecsSinceEpoch() - startTime);
  if(took > 3)
    qDebug() << "took : " << (QDateTime::currentMSecsSinceEpoch() - startTime) << "ms ";
}

// Update and render
void OpenGLWidget::updateScene() {
    static float s;
    static float t;
    s += 2;
    t += 0.03;

    armLink1->rotation.setZ(-serialData[1]);
    armLink2->rotation.setY(-serialData[2] + 90);
    armLink3->rotation.setY(-serialData[3] - 90);
    armLink4->rotation.setX(-serialData[4]);

    // Update models
    root->computeModels(NULL);

    // Keep on arm
    QVector3D camPos = armLink4->model * QVector4D(-0.3, 0.5, 0, 1).toVector3D();
    cameraArm->position = camPos;

    QVector3D camPosUp = (armLink4->model * QVector4D(-0.3, 0.5, 1, 1).toVector3D() - camPos);
    camPosUp.normalize();
    cameraArm->up = camPosUp;

    // Follow tip of arm
    QVector3D armPos = armLink4->model * QVector4D(0, 0.1, 0, 1).toVector3D();
    cameraArm->center = armPos;
    cameraArm->update();

    camera->animate(0.03);

    if(!camera->completed || !MainWindow::wantsChangeCamera){
        // Does nothing if didn't completed camera change or not wants camera change
    }else if(camera->completed && camera->to == cameraArm){
        camera->gotoCamera(cameraUp, 3);
        MainWindow::wantsChangeCamera = false;
    }else if(camera->completed && camera->to == cameraStatic){
        camera->gotoCamera(cameraArm, 3);
        MainWindow::wantsChangeCamera = false;
    }else if(camera->completed && camera->to == cameraUp){
        camera->gotoCamera(cameraStatic, 3);
        MainWindow::wantsChangeCamera = false;
    }

    update();
}
