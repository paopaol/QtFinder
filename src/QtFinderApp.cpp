#include "QtFinderApp.h"
#include "QtFinderApp_p.h"

QtFinderApp::QtFinderApp(QWidget *parent)
    : QWidget(parent), d_ptr(new QtFinderAppPrivate()) {}

QtFinderApp::~QtFinderApp() noexcept {}

void QtFinderApp::setFinderTool(AbstractQtFinderToolPtr &finderTool) {
  Q_ASSERT(finderTool);
  Q_D(QtFinderApp);
  d->tool_.swap(finderTool);
}

void QtFinderApp::setFinderWindow(AbstractQtFinderWindowPtr &finderWindow) {
  Q_ASSERT(finderWindow);
  Q_D(QtFinderApp);
  d->win_.swap(finderWindow);
}

void QtFinderApp::setDesktopService(AbstractDesktopServicePtr &desktopService) {
  Q_ASSERT(desktopService);
  Q_D(QtFinderApp);
  d->desktopService_.swap(desktopService);
}

void QtFinderApp::setFileSystemScanner(
    AbstractFileSystemScannerPtr &fileSystemScanner) {
  Q_ASSERT(fileSystemScanner);
  Q_D(QtFinderApp);
  d->fileSystemScanner_.swap(fileSystemScanner);
}

void QtFinderApp::startSearch(const QtFinder::Cmd cmd,
                              const QString &keywords) {
  Q_D(QtFinderApp);
  d->prepare();
  d->win_->setSearchKeywords(cmd, keywords);
}
