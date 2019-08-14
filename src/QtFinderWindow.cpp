#include <Events.h>
#include <QDesktopServices>
#include <QDir>
#include <QFileInfo>
#include <QUrl>
#include <QtFinderWindow.h>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

QtFinderWindow::QtFinderWindow(QWidget *parent) : QWidget(parent) {
  ui.setupUi(this);
  ui.promptLabel->setText(directory_);

  connect(ui.searchLineEdit, &QtFinder::SearchLineEdit::searchKeywordsChanged,
          this, &QtFinderWindow::searchKeywordsChanged);
  connect(ui.searchLineEdit, &QtFinder::SearchLineEdit::keywordsEmpty, this,
          &QtFinderWindow::keywordsEmpty);
  connect(this, &QtFinderWindow::shortcutKeyPressed, this,
          &QtFinderWindow::shortcutKeyHandler);
}

QtFinderWindow::~QtFinderWindow() noexcept {}

void QtFinderWindow::keyPressEvent(QKeyEvent *event) {
  auto key = key_press_event(event);
  if (key != Qt::Key_unknown) {
    emit shortcutKeyPressed(key);
    return;
  }
  QWidget::keyPressEvent(event);
}

void QtFinderWindow::setSearchKeywords(const QtFinder::Cmd cmd,
                                       const QString &keywords) {
  auto keywordsWithCmd = toQString(cmd) + " " + keywords;
  ui.searchLineEdit->setText(keywordsWithCmd);
}

void QtFinderWindow::setFdCmdTriggerDelay(int delayMs) {
  ui.searchLineEdit->setFdCmdTriggerDelay(delayMs);
}

void QtFinderWindow::clearSearchKeywords() { ui.searchLineEdit->clear(); }

QString QtFinderWindow::currentDirectory() const {
  auto path = directory_ == "~" ? QDir::homePath() : directory_;
  return path;
}

void QtFinderWindow::setCurrentDirectory(const QString &absolutePath) {
  auto path = QDir::fromNativeSeparators(absolutePath);
  auto home = QDir::homePath();

  path = path == home ? "~" : path;
  directory_ = path;
  ui.promptLabel->setText(directory_);
}

void QtFinderWindow::selectCandidateAsFile(int row) {
  auto text = selectCandidate(row);
  if (!text.isEmpty()) {
    emit selectedFilechanged(text);
  }
}

void QtFinderWindow::selectCandidateAsDirectory(int row) {
  auto text = selectCandidate(row);
  if (!text.isEmpty()) {
    emit selectedDirectoryChanged(text);
  }
}

void QtFinderWindow::addCandidates(const QStringList &candidates) {
  ui.quickfixWidget->addCandidates(candidates);
}

void QtFinderWindow::setCandidates(const QStringList &candidates) {
  ui.quickfixWidget->clear();
  addCandidates(candidates);
}

int QtFinderWindow::candidateSize() const { return ui.quickfixWidget->count(); }

QString QtFinderWindow::selectCandidate(int row) {
  auto item = ui.quickfixWidget->item(row);
  if (!item) {
    emit candidateEmpty();
    return "";
  }
  auto text = item->text();
  if (text.isEmpty()) {
    return "";
  }
  /// the text maybe a relative path or a absolute path,
  /// but we need convert relative path to absolute path
  QFileInfo fileInfo(text);
  if (!fileInfo.isAbsolute()) {
    fileInfo = currentDirectory() + "/" + text;
  }
  text = fileInfo.absoluteFilePath();
  return text;
}

void QtFinderWindow::focusRow(int row) {
  if (ui.quickfixWidget->count() == 0) {
    return;
  }
  ui.quickfixWidget->setCurrentRow(row);
  ui.quickfixWidget->scrollToItem(ui.quickfixWidget->currentItem());
}

void QtFinderWindow::shortcutKeyHandler(Qt::Key key) {
  switch (key) {
  case Qt::Key_Enter | Qt::Key_Control: {
    selectCandidateAsDirectory(ui.quickfixWidget->currentRow());
    break;
  }
  case Qt::Key_Enter: {
    selectCandidateAsFile(ui.quickfixWidget->currentRow());
    break;
  }
  case Qt::Key_Down: {
    ui.quickfixWidget->focusNextCandidate();
    break;
  }
  case Qt::Key_Up: {
    ui.quickfixWidget->focusPreviousCandidate();
    break;
  }
  case Qt::Key_H | Qt::Key_Control: {
    QDir dir(currentDirectory());
    dir.cdUp();
    emit searchKeywordsChanged(QtFinder::Cmd::kDirectoryChanged,
                               QStringList() << dir.absolutePath());
    break;
  }
  }
}

static QString fromQtFinderCmd(QtFinder::Cmd cmd) { return ""; }
