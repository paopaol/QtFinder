#include <QDebug>
#include <QTextCodec>
#include <QtFinderWindow.h>

static QString fdPattern(const QStringList &keywords);
static QStringList rgPattern(const QStringList &keywords);
static void killProcess(QProcess &process);

QtFinderWindow::QtFinderWindow(QWidget *parent) : QWidget(parent) {
  uiWidget.setupUi(this);
  connect(uiWidget.searchLineEdit, &SearchLineEdit::searchKeyWordsChanged, this,
          &QtFinderWindow::onSearchKeyWordsChanged);
  connect(&rg_, &QProcess::readyReadStandardOutput, this, [&]() {
    QTextCodec *textCodec = QTextCodec::codecForName("UTF8");
    QString line = textCodec->fromUnicode(rg_.readLine());
    printf("%s", line.toStdString().c_str());
  });
  connect(&rg_, &QProcess::readyReadStandardError, [&]() {
    QTextCodec *textCodec = QTextCodec::codecForName("UTF8");
    QString line = textCodec->fromUnicode(rg_.readAllStandardError());
    qDebug() << line;
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

  rg_.start("rg", rgPattern(keywords));
  rg_.waitForStarted();

  fd_.start("fd", QStringList() << "-p" << fdPattern(keywords) << "~");
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
    patterns.push_back("-e");
    patterns.push_back(key);
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
