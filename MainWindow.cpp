#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "EasyGL/EasyScene.h"

MainWindow* MainWindow::instance = NULL;
bool MainWindow::wantsChangeCamera = false;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  instance = this;
}

MainWindow::~MainWindow() {
  delete ui;
}


// Listen to keypresses
void MainWindow::keyPressEvent(QKeyEvent *ev) {
    qDebug() << "Pressed: " << ev->key();
    if(ev->key() == Qt::Key_Space)
        MainWindow::wantsChangeCamera = true;
}
