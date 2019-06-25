#include <QtFinderWindow.h>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  QtFinderApp qtFinder;

  qtFinder.run();

  return app.exec();
}
