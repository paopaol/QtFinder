#include "QtFinderWindowImpl.h"
#include <QApplication>
#include <QtFinderApp.h>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

   auto finderApp = QtFinderApp::newApp();
   finderApp->run();
   finderApp->show();

  return app.exec();
}
