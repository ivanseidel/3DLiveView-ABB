#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  static MainWindow* instance;
  Ui::MainWindow *ui;

  static bool wantsChangeCamera;

protected:
  void keyPressEvent(QKeyEvent *ev) Q_DECL_OVERRIDE;
};

#endif // MAINWINDOW_H
