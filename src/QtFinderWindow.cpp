#include <QDebug>
#include <QDir>
#include <QStringListModel>
#include <QTextCodec>
#include <QtFinderWindow.h>

static QString fdPattern(const QStringList &keywords);
static QStringList rgPattern(const QStringList &keywords);
static void killProcess(QProcess &process);
static QStringList directoryEntryList(const QString &directory);

QtFinderWindow::QtFinderWindow(QWidget *parent) : QWidget(parent) {
  ui.setupUi(this);

  connect(ui.searchLineEdit, &SearchLineEdit::searchKeyWordsChanged, this,
          &QtFinderWindow::onSearchKeyWordsChanged);
  connect(ui.searchLineEdit, &SearchLineEdit::directoryChanged, this,
          &QtFinderWindow::onDirectoryChanged);
  connect(ui.searchLineEdit, &SearchLineEdit::keywordsEmpty, this,
          [&]() { listDirectory(directory_); });
  connect(ui.searchLineEdit, &SearchLineEdit::ctrlNextPressed, this, [&]() {
    ui.quickfixWidget->updateCurrentRow(QuickfixWidget::SelectOpt::kDown);
  });
  connect(ui.searchLineEdit, &SearchLineEdit::ctrlPrevPressed, this, [&]() {
    ui.quickfixWidget->updateCurrentRow(QuickfixWidget::SelectOpt::kUp);
  });

  connect(&rg_, &QProcess::readyRead, this, [&]() {
    QTextCodec *textCodec = QTextCodec::codecForName("UTF8");
    while (rg_.canReadLine()) {
      QString line = rg_.readLine();
      ui.quickfixWidget->addItem(line);
    }
    ui.quickfixWidget->updateCurrentRow(QuickfixWidget::SelectOpt::kKeep);
  });
  connect(&rg_, &QProcess::readyReadStandardError, [&]() {
    QTextCodec *textCodec = QTextCodec::codecForName("UTF8");
    QString line = textCodec->fromUnicode(rg_.readAllStandardError());
    qDebug() << line;
  });
}
QtFinderWindow::~QtFinderWindow() noexcept {}

void QtFinderWindow::show() {
  QWidget::show();
  listDirectory();
}

void QtFinderWindow::onSearchKeyWordsChanged(
    const QStringList &keywords, SearchLineEdit::SearchRequest type) {
  switch (type) {
  case SearchLineEdit::SearchRequest::kFd: {
    // fdSearch(keywords);
    break;
  }
  case SearchLineEdit::SearchRequest::kQuickfix: {
    // quickfixSearch(keywords);
    break;
  }
  default:
    break;
  }
  qDebug() << keywords;
}

void QtFinderWindow::search(const QStringList &keywords) {
  ui.quickfixWidget->clear();
  killProcess(fd_);
  killProcess(rg_);

  fd_.setStandardOutputProcess(&rg_);

  rg_.start("rg", rgPattern(keywords));
  rg_.waitForStarted();

  QStringList fdArgs;
  fdArgs << "-p" << fdPattern(keywords) << directory_;

  fd_.start("fd", fdArgs);
  fd_.waitForStarted();
  qDebug() << fd_.arguments();
}

void QtFinderWindow::onDirectoryChanged(const QString &directory) {
  directory_ = directory;
  listDirectory(directory_);
}

void QtFinderWindow::listDirectory(const QString &directory) {
  ui.quickfixWidget->clear();
  ui.promptLabel->setText(directory);
  auto entrys =
      directoryEntryList(directory == "~" ? QDir::homePath() : directory);
  ui.quickfixWidget->addItems(entrys);
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

static QStringList directoryEntryList(const QString &directory) {
  QDir dir(directory);
  return dir.entryList();
}
