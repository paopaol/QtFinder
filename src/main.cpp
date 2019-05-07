#include <QtFinderWindow.h>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  QtFinderWindow qtFinder;

  qtFinder.show();

  return app.exec();
}
