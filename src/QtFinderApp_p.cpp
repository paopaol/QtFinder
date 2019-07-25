#include "QtFinderApp_p.h"
#include "QtFinderApp.h"
#include <QFileInfo>
#include <QObject>

void QtFinderAppPrivate::prepare() {
  Q_Q(QtFinderApp);
  connect(win_.data(), &AbstractQtFinderWindow::searchKeywordsChanged, this,
          &QtFinderAppPrivate::onSearchKeywordsChanged);
  connect(tool_.data(), &AbstractQtFinderTool::candidateReady, win_.data(),
          &AbstractQtFinderWindow::addCandidate);
  connect(win_.data(), &AbstractQtFinderWindow::selectedFilechanged, this,
          &QtFinderAppPrivate::onSelectedFileChanged);
}

void QtFinderAppPrivate::onSearchKeywordsChanged(QtFinder::Cmd cmd,
                                                 const QStringList &keywords) {
  switch (cmd) {
  case QtFinder::Cmd::kFd:
    tool_->startSearchOnDirectory(win_->currentDirectory(), keywords);
    break;
  }
}

void QtFinderAppPrivate::onSelectedFileChanged(const QString &file) {
  QFileInfo fileInfo(file);
  if (!fileInfo.isDir()) {
    desktopService_->openFile(file);
    return;
  }

  win_->setCurrentDirectory(file);
  auto files = fileSystemScanner_->scanDirectory(win_->currentDirectory());
  win_->setCandidates(files);
}
