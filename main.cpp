#include "MainWindow.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
  QSurfaceFormat format;
  format.setVersion(4, 1);

#if __APPLE__
  format.setProfile(QSurfaceFormat::CoreProfile);
#else
  format.setProfile(QSurfaceFormat::CompatibilityProfile);
#endif

  format.setDepthBufferSize(24);
  format.setSamples(4);
  QSurfaceFormat::setDefaultFormat(format);

  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  return a.exec();
}
