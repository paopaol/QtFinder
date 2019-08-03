#include "QtFinderApp.h"
#include "QtFinderApp_p.h"

QtFinderApp::QtFinderApp(QWidget *parent) : QWidget(parent) {
  d_ptr.reset(new QtFinderAppPrivate(this));
}

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
  d->layout_.addWidget(d->win_.data());
  d->layout_.setMargin(0);
  setLayout(&d->layout_);
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

void QtFinderApp::run() {
  Q_D(QtFinderApp);
  d->prepare();
}
