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
}

QtFinderWindowPrivate::~QtFinderWindowPrivate() noexcept {}

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

void QtFinderWindowPrivate::openCandidateAsPath(int index) {
  auto item = ui.quickfixWidget->currentItem();
  if (!item) {
    emit candidateEmpty();
    return;
  }
  auto text = item->text();

  /// the text maybe a relative path or a absolute path,
  /// but we need convert relative path to absolute path
  QFileInfo fileInfo(text);
  if (!fileInfo.isAbsolute()) {
    fileInfo = directory_ + "/" + text;
  }
  emit openPathChanged(fileInfo.absoluteFilePath());
}

static QString fromQtFinderCmd(QtFinder::Cmd cmd) { return ""; }
