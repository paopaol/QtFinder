#include "QtFinderApp_p.h"
#include "QtFinderApp.h"
#include <QObject>

void QtFinderAppPrivate::prepare() {
  connect(win_.data(), &AbstractQtFinderWindow::searchKeywordsChanged, this,
          &QtFinderAppPrivate::onSearchKeywordsChanged);
  connect(tool_.data(), &AbstractQtFinderTool::candidateReady, win_.data(),
          &AbstractQtFinderWindow::addCandidate);
}

void QtFinderAppPrivate::onSearchKeywordsChanged(QtFinder::Cmd cmd,
                                                 const QStringList &keywords) {
  tool_->startSearchOnDirectory(win_->currentDirectory(), keywords);
}
