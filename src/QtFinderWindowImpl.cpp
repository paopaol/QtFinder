#include "QtFinderWindowImpl.h"

QtFinderWindowImpl::QtFinderWindowImpl(QWidget *parent)
    : AbstractQtFinderWindow(parent), win_(this), layout_(this) {
  connect(&win_, &QtFinderWindow::searchKeywordsChanged, this,
          &AbstractQtFinderWindow::searchKeywordsChanged);
  connect(&win_, &QtFinderWindow::keywordsEmpty, this,
          &AbstractQtFinderWindow::keywordsEmpty);
  connect(&win_, &QtFinderWindow::selectedFilechanged, this,
          &AbstractQtFinderWindow::selectedFilechanged);
  connect(&win_, &QtFinderWindow::selectedDirectoryChanged, this,
          &AbstractQtFinderWindow::selectedDirectoryChanged);
  connect(&win_, &QtFinderWindow::candidateEmpty, this,
          &AbstractQtFinderWindow::candidateEmpty);
  layout_.addWidget(&win_);
  layout_.setMargin(0);
  setLayout(&layout_);
}

QtFinderWindowImpl::~QtFinderWindowImpl() {}

void QtFinderWindowImpl::setSearchKeywords(QtFinder::Cmd cmd,
                                           const QString &keywords) {
  win_.setSearchKeywords(cmd, keywords);
}

void QtFinderWindowImpl::setFdCmdTriggerDelay(int delayMs) {
  win_.setFdCmdTriggerDelay(delayMs);
}

void QtFinderWindowImpl::clearSearchKeywords() { win_.clearSearchKeywords(); }

QString QtFinderWindowImpl::currentDirectory() const {
  return win_.currentDirectory();
}

void QtFinderWindowImpl::setCurrentDirectory(const QString &absolutePath) {
  win_.setCurrentDirectory(absolutePath);
}

void QtFinderWindowImpl::selectCandidateAsFile(int row) {
  win_.selectCandidateAsFile(row);
}

void QtFinderWindowImpl::selectCandidateAsDirectory(int row) {
  win_.selectCandidateAsDirectory(row);
}

void QtFinderWindowImpl::addCandidates(const QStringList &candidates) {
  win_.addCandidates(candidates);
}

void QtFinderWindowImpl::setCandidates(const QStringList &candidates) {
  win_.setCandidates(candidates);
}

int QtFinderWindowImpl::candidateSize() const { return win_.candidateSize(); }

void QtFinderWindowImpl::focusRow(int row) { win_.focusRow(row); }
