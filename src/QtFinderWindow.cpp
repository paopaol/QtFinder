#include <QDebug>
#include <QtFinderWindow.h>

static QString fdPattern(const QStringList &keywords);
static QStringList rgPattern(const QStringList &keywords);
static void killProcess(QProcess &process);

QtFinderWindow::QtFinderWindow(QWidget *parent) : QWidget(parent) {
  uiWidget.setupUi(this);
  connect(uiWidget.searchLineEdit, &SearchLineEdit::searchKeyWordsChanged, this,
          &QtFinderWindow::onSearchKeyWordsChanged);
  connect(&rg_, &QProcess::readyReadStandardOutput, this, [&]() {
    // while (rg_.isReadable()) {
    qDebug() << rg_.readAll();
    // }
  });
}
QtFinderWindow::~QtFinderWindow() noexcept {}

void QtFinderWindow::onSearchKeyWordsChanged(const QStringList &keywords) {
  qDebug() << keywords;
  search(keywords);
}

void QtFinderWindow::search(const QStringList &keywords) {
  killProcess(fd_);
  killProcess(rg_);

  fd_.setStandardOutputProcess(&rg_);

  rg_.start("rg", QStringList() << rgPattern(keywords));
  rg_.waitForStarted();

  fd_.start("fd", QStringList() << "-p" << fdPattern(keywords) << "/opt");
  qDebug() << fd_.arguments();
  qDebug() << rg_.arguments();
  fd_.waitForStarted();
}

static QString fdPattern(const QStringList &keywords) {
  QString pattern;
  QTextStream st(&pattern);
  for (auto &key : keywords) {
    st << key << ".*";
  }
  return pattern;
}

static QStringList rgPattern(const QStringList &keywords) {
  QStringList patterns;
  for (auto &key : keywords) {
    patterns << " -e " << key;
  }
  return patterns;
}

static void killProcess(QProcess &process) {
  if (process.state() == QProcess::ProcessState::Starting) {
    process.waitForStarted();
  }

  if (process.state() == QProcess::ProcessState::Running) {
    process.kill();
    process.waitForFinished();
  }
}
