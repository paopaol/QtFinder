#include "QtFinderApp_p.h"
#include "QtFinderApp.h"
#include <QObject>

void QtFinderAppPrivate::prepare() {
  Q_Q(QtFinderApp);
  connect(win_.data(), &AbstractQtFinderWindow::searchKeywordsChanged, this,
          &QtFinderAppPrivate::onSearchKeywordsChanged);
  connect(tool_.data(), &AbstractQtFinderTool::candidateReady, win_.data(),
          &AbstractQtFinderWindow::addCandidate);
  connect(win_.data(), &AbstractQtFinderWindow::selectedFilechanged,
          desktopService_.data(), &AbstractDesktopService::openFile);
}

void QtFinderAppPrivate::onSearchKeywordsChanged(QtFinder::Cmd cmd,
                                                 const QStringList &keywords) {
  tool_->startSearchOnDirectory(win_->currentDirectory(), keywords);
}
