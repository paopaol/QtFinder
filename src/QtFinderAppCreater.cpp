#include "DesktopServiceImpl.h"
#include "FileSystemScannerImpl.h"
#include "QtFinderToolImpl.h"
#include "QtFinderWindowImpl.h"
#include <QtFinderApp.h>

std::unique_ptr<QtFinderApp> QtFinderApp::newApp(QWidget *parent) {
  std::unique_ptr<QtFinderApp> app = std::make_unique<QtFinderApp>(parent);

  QScopedPointer<AbstractQtFinderTool> tool(new QtFinderToolImpl(app.get()));
  QScopedPointer<AbstractQtFinderWindow> win(new QtFinderWindowImpl());
  QScopedPointer<AbstractDesktopService> desktopService(
      new DesktopServiceImpl(app.get()));
  QScopedPointer<AbstractFileSystemScanner> fileSystemScanner(
      new FileSystemScannerImpl(app.get()));

  win->setMinimumSize(QSize(800, 600));
  app->setFinderTool(tool);
  app->setFinderWindow(win);
  app->setDesktopService(desktopService);
  app->setFileSystemScanner(fileSystemScanner);

  return app;
}
