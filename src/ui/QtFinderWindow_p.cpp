#include <private/QtFinderWindow_p.h>

QtFinderWindowPrivate::QtFinderWindowPrivate(QWidget *parent)
    : QWidget(parent) {
  ui.setupUi(this);
  ui.promptLabel->setText(directory_);
}

QtFinderWindowPrivate::~QtFinderWindowPrivate() noexcept {}


void QtFinderWindowPrivate::setSearchKeywords(const QtFinder::Cmd cmd,
                                              const QString &keywords) {
  ui.searchLineEdit->setText(keywords);
}

QString QtFinderWindowPrivate::currentDirectory() const { return directory_; }

static QString fromQtFinderCmd(QtFinder::Cmd cmd) { return ""; }
