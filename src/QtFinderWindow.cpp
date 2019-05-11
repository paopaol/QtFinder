#include <QDebug>
#include <QStringListModel>
#include <QTextCodec>
#include <QtFinderWindow.h>

static QString fdPattern(const QStringList &keywords);
static QStringList rgPattern(const QStringList &keywords);
static void killProcess(QProcess &process);

QtFinderWindow::QtFinderWindow(QWidget *parent) : QWidget(parent) {
  ui.setupUi(this);

  connect(&delayTimer_, &QTimer::timeout, this, [&]() { search(keyWords_); });
  delayTimer_.setSingleShot(true);

  connect(ui.searchLineEdit, &SearchLineEdit::searchKeyWordsChanged, this,
          &QtFinderWindow::onSearchKeyWordsChanged);
  connect(ui.searchLineEdit, &SearchLineEdit::directoryChanged, this,
          &QtFinderWindow::onDirectoryChanged);
  connect(
      ui.searchLineEdit, &SearchLineEdit::ctrlNextPressed, this, [&]() {
        ui.listWidget->updateCurrentRow(QuickfixWidget::SelectOpt::kDown);
      });
  connect(
      ui.searchLineEdit, &SearchLineEdit::ctrlPrevPressed, this, [&]() {
        ui.listWidget->updateCurrentRow(QuickfixWidget::SelectOpt::kUp);
      });

  connect(&rg_, &QProcess::readyRead, this, [&]() {
    QTextCodec *textCodec = QTextCodec::codecForName("UTF8");
    while (rg_.canReadLine()) {
      QString line = rg_.readLine();
      ui.listWidget->addItem(line);
    }
    ui.listWidget->updateCurrentRow(QuickfixWidget::SelectOpt::kKeep);
  });
  connect(&rg_, &QProcess::readyReadStandardError, [&]() {
    QTextCodec *textCodec = QTextCodec::codecForName("UTF8");
    QString line = textCodec->fromUnicode(rg_.readAllStandardError());
    qDebug() << line;
  });
  search(QStringList() << directory_, true);
}
QtFinderWindow::~QtFinderWindow() noexcept {}

void QtFinderWindow::onSearchKeyWordsChanged(const QStringList &keywords) {
  qDebug() << keywords;
  keyWords_ = keywords;

  /// canel search timer,and restart the timer
  /// If the user no longer enters, a delay will trigger the search.
  delayTimer_.stop();
  if (keyWords_.empty()) {
    search(QStringList() << directory_, true);
    return;
  }
  delayTimer_.start(searchDelay_);
}

void QtFinderWindow::search(const QStringList &keywords, bool depth) {
  ui.listWidget->clear();
  killProcess(fd_);
  killProcess(rg_);

  fd_.setStandardOutputProcess(&rg_);

  rg_.start("rg", rgPattern(keywords));
  rg_.waitForStarted();

  QStringList fdArgs;
  if (depth) {
    fdArgs << "-d"
           << "1"
           << "-p" << keywords << directory_;
  } else {
    fdArgs << "-p" << fdPattern(keywords) << directory_;
  }
  fd_.start("fd", fdArgs);
  fd_.waitForStarted();
  qDebug() << fd_.arguments();
}

void QtFinderWindow::onDirectoryChanged(const QString &directory) {
  directory_ = directory;
  ui.promptLabel->setText(directory_);
  search(QStringList() << directory, true);
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
