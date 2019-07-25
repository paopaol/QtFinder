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
  d->tool_->setParent(this);
}

void QtFinderApp::setFinderWindow(AbstractQtFinderWindowPtr &finderWindow) {
  Q_ASSERT(finderWindow);
  Q_D(QtFinderApp);
  d->win_.swap(finderWindow);
  d->win_->setParent(this);
}

void QtFinderApp::setDesktopService(AbstractDesktopServicePtr &desktopService) {
  Q_ASSERT(desktopService);
  Q_D(QtFinderApp);
  d->desktopService_.swap(desktopService);
  d->desktopService_->setParent(this);
}

void QtFinderApp::setFileSystemScanner(
    AbstractFileSystemScannerPtr &fileSystemScanner) {
  Q_ASSERT(fileSystemScanner);
  Q_D(QtFinderApp);
  d->fileSystemScanner_.swap(fileSystemScanner);
  d->fileSystemScanner_->setParent(this);
}

void QtFinderApp::run() {
  Q_D(QtFinderApp);
  d->prepare();
}

void QtFinderApp::keyPressEvent(QKeyEvent *e) { QWidget::keyPressEvent(e); }
