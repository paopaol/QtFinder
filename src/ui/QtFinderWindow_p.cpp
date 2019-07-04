#include <private/QtFinderWindow_p.h>

QtFinderWindowPrivate::QtFinderWindowPrivate(QWidget *parent)
    : QWidget(parent) {
  ui.setupUi(this);
  ui.promptLabel->setText(directory_);

  connect(ui.searchLineEdit, &QtFinder::SearchLineEdit::searchKeywordsChanged,
          this, &QtFinderWindowPrivate::searchKeywordsChanged);
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

QString QtFinderWindowPrivate::currentDirectory() const { return directory_; }

static QString fromQtFinderCmd(QtFinder::Cmd cmd) { return ""; }
