#include "QtFinderApp_p.h"
#include "QtFinderApp.h"
#include <QFileInfo>
#include <QObject>

void QtFinderAppPrivate::prepare() {
  connect(win_.data(), &AbstractQtFinderWindow::searchKeywordsChanged, this,
          &QtFinderAppPrivate::onSearchKeywordsChanged);
  connect(tool_.data(), &AbstractQtFinderTool::candidatesReady, win_.data(),
          &AbstractQtFinderWindow::addCandidates);
  connect(tool_.data(), &AbstractQtFinderTool::stopped, this,
          [&]() { win_->focusRow(0); });
  connect(win_.data(), &AbstractQtFinderWindow::selectedFilechanged, this,
          &QtFinderAppPrivate::onSelectedFileChanged);
  connect(win_.data(), &AbstractQtFinderWindow::selectedDirectoryChanged,
          desktopService_.data(), &AbstractDesktopService::openWithFileManager);
  auto entrys = fileSystemScanner_->scanDirectory(win_->currentDirectory());
  win_->setCandidates(entrys);
}

void QtFinderAppPrivate::onSearchKeywordsChanged(QtFinder::Cmd cmd,
                                                 const QStringList &keywords) {
  switch (cmd) {
  case QtFinder::Cmd::kFd:
    win_->setCandidates(QStringList());
    tool_->startSearchOnDirectory(win_->currentDirectory(), keywords);
    break;
  case QtFinder::Cmd::kDirectoryChanged:
    win_->setCurrentDirectory(keywords.front());
    auto entrys = fileSystemScanner_->scanDirectory(win_->currentDirectory());
    win_->setCandidates(entrys);
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
