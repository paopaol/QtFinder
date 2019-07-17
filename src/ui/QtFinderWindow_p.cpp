#include <Events.h>
#include <QDesktopServices>
#include <QFileInfo>
#include <QUrl>
#include <private/QtFinderWindow_p.h>

QtFinderWindowPrivate::QtFinderWindowPrivate(QWidget *parent)
    : QWidget(parent) {
  ui.setupUi(this);
  ui.promptLabel->setText(directory_);

  connect(ui.searchLineEdit, &QtFinder::SearchLineEdit::searchKeywordsChanged,
          this, &QtFinderWindowPrivate::searchKeywordsChanged);
  connect(ui.searchLineEdit, &QtFinder::SearchLineEdit::keywordsEmpty, this,
          &QtFinderWindowPrivate::keywordsEmpty);
  // connect(ui.searchLineEdit, &QtFinder::SearchLineEdit::shortcutKeyPressed,
  //         this, &QtFinderWindowPrivate::shortcutKeyHandler);
  // connect(ui.quickfixWidget, &QtFinder::QuickfixWidget::shortcutKeyPressed,
  //         this, &QtFinderWindowPrivate::shortcutKeyHandler);
  connect(this, &QtFinderWindowPrivate::shortcutKeyPressed, this,
          &QtFinderWindowPrivate::shortcutKeyHandler);
}

QtFinderWindowPrivate::~QtFinderWindowPrivate() noexcept {}

void QtFinderWindowPrivate::keyPressEvent(QKeyEvent *event) {
  auto key = key_press_event(event);
  if (key != Qt::Key_unknown) {
    emit shortcutKeyPressed(key);
    return;
  }
  QWidget::keyPressEvent(event);
}

void QtFinderWindowPrivate::setSearchKeywords(const QtFinder::Cmd cmd,
                                              const QString &keywords) {
  auto keywordsWithCmd = toQString(cmd) + " " + keywords;
  ui.searchLineEdit->setText(keywordsWithCmd);
}

void QtFinderWindowPrivate::setFdCmdTriggerDelay(int delayMs) {
  ui.searchLineEdit->setFdCmdTriggerDelay(delayMs);
}

void QtFinderWindowPrivate::clearSearchKeywords() {
  ui.searchLineEdit->clear();
}

QString QtFinderWindowPrivate::currentDirectory() const { return directory_; }

void QtFinderWindowPrivate::selectCandidateAsPath(int row) {
  auto text = selectCandidate(row);
  if (text.isEmpty()) {
    return;
  }
  /// the text maybe a relative path or a absolute path,
  /// but we need convert relative path to absolute path
  QFileInfo fileInfo(text);
  if (!fileInfo.isAbsolute()) {
    fileInfo = directory_ + "/" + text;
  }
  emit selectedPathChanged(fileInfo.absoluteFilePath());
}

void QtFinderWindowPrivate::addCandidate(const QString &candidate) {
  ui.quickfixWidget->addCandidate(candidate);
}

int QtFinderWindowPrivate::candidateSize() {
  return ui.quickfixWidget->count();
}

QString QtFinderWindowPrivate::selectCandidate(int row) {
  auto item = ui.quickfixWidget->item(row);
  if (!item) {
    emit candidateEmpty();
    return "";
  }
  auto text = item->text();
  return text;
}

void QtFinderWindowPrivate::shortcutKeyHandler(Qt::Key key) {
  switch (key) {
  case Qt::Key_Enter | Qt::Key_Control: {
    selectCandidateAsPath(ui.quickfixWidget->currentRow());
    break;
  }
  case Qt::Key_Down: {
    ui.quickfixWidget->moveNextCandidate();
    break;
  }
  case Qt::Key_Up: {
    ui.quickfixWidget->movePreviousCandidate();
    break;
  }
  }
}

static QString fromQtFinderCmd(QtFinder::Cmd cmd) { return ""; }
