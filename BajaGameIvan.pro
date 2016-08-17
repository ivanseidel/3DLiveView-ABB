#-------------------------------------------------
#
# Project created by QtCreator 2016-08-01T18:01:51
#
#-------------------------------------------------

QT       += core gui opengl serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BajaGameIvan
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    EasyGL/EasyGeometry.cpp \
    EasyGL/EasyMaterial.cpp \
    EasyGL/EasyMesh.cpp \
    EasyGL/EasyScene.cpp \
    OpenGLWidget.cpp \
    BasicMaterial.cpp \
    EasyGL/EasyCamera.cpp \
    EasyGL/EasyLight.cpp \
    EasyGL/EasyCameraTransition.cpp \
    SquareGeometry.cpp

HEADERS  += MainWindow.h \
    EasyGL/EasyGeometry.h \
    EasyGL/EasyMaterial.h \
    EasyGL/EasyMesh.h \
    EasyGL/EasyScene.h \
    OpenGLWidget.h \
    BasicMaterial.h \
    EasyGL/EasyCamera.h \
    EasyGL/EasyLight.h \
    EasyGL/EasyCameraTransition.h \
    SquareGeometry.h


FORMS    += mainwindow.ui

INCLUDEPATH += $$PWD/models

DISTFILES += \
    models/Apple.off \
    models/armadillo.off \
    models/beetle.off \
    models/bunny.off \
    models/camel.off \
    models/elephant.off \
    models/elk.off \
    models/mannequin.off \
    models/mushroom.off \
    models/pear.off \
    models/pig.off \
    models/seashell.off \
    models/sphere.off \
    models/torus.off \
    shaders/fshader1.glsl \
    shaders/vshader1.glsl \
    shaders/basicmaterial_vshader.glsl \
    shaders/basicmaterial_fshader.glsl \
    shaders/background_fshader.glsl \
    shaders/background_vshader.glsl

RESOURCES += \
    resources.qrc
