#include <QDebug>
#include <QStringListModel>
#include <QTextCodec>
#include <QtFinderWindow.h>

static QString fdPattern(const QStringList &keywords);
static QStringList rgPattern(const QStringList &keywords);
static void killProcess(QProcess &process);

QtFinderWindow::QtFinderWindow(QWidget *parent) : QWidget(parent) {
  uiWidget.setupUi(this);
  connect(uiWidget.searchLineEdit, &SearchLineEdit::searchKeyWordsChanged, this,
          &QtFinderWindow::onSearchKeyWordsChanged);
  connect(uiWidget.searchLineEdit, &SearchLineEdit::directoryChanged, this,
          &QtFinderWindow::onDirectoryChanged);

  connect(&rg_, &QProcess::readyReadStandardOutput, this, [&]() {
    QTextCodec *textCodec = QTextCodec::codecForName("UTF8");
    QString line = textCodec->fromUnicode(rg_.readLine());

    uiWidget.listWidget->addItem(line);
  });
  connect(&rg_, &QProcess::readyReadStandardError, [&]() {
    QTextCodec *textCodec = QTextCodec::codecForName("UTF8");
    QString line = textCodec->fromUnicode(rg_.readAllStandardError());
    qDebug() << line;
  });
  connect(&delayTimer_, &QTimer::timeout, this, [&]() { search(keyWords_); });
  delayTimer_.setSingleShot(true);
}
QtFinderWindow::~QtFinderWindow() noexcept {}

void QtFinderWindow::onSearchKeyWordsChanged(const QStringList &keywords) {
  qDebug() << keywords;
  keyWords_ = keywords;

  /// canel search timer,and restart the timer
  /// If the user no longer enters, a delay will trigger the search.
  delayTimer_.stop();
  if (keyWords_.empty()) {
    return;
  }
  delayTimer_.start(searchDelay_);
}

void QtFinderWindow::search(const QStringList &keywords) {
  uiWidget.listWidget->clear();
  killProcess(fd_);
  killProcess(rg_);

  fd_.setStandardOutputProcess(&rg_);

  rg_.start("rg", rgPattern(keywords));
  rg_.waitForStarted();

  fd_.start("fd", QStringList() << "-p" << fdPattern(keywords) << directory_);
  fd_.waitForStarted();
}

void QtFinderWindow::onDirectoryChanged(const QString &directory) {
  directory_ = directory;
  uiWidget.promptLabel->setText(directory_);
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
