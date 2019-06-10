#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QFileInfo>
#include <QStringListModel>
#include <QTextCodec>
#include <QUrl>
#include <QtFinderWindow.h>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem::v1;

static QString fdPattern(const QStringList &keywords);
static QStringList rgPattern(const QStringList &keywords);
static void killProcess(QProcess &process);
static QStringList directoryEntryList(const QString &directory);

QtFinderWindow::QtFinderWindow(QWidget *parent) : QWidget(parent) {
  ui.setupUi(this);

  connect(ui.searchLineEdit, &SearchLineEdit::searchKeyWordsChanged, this,
          &QtFinderWindow::onSearchKeyWordsChanged);
  connect(ui.searchLineEdit, &SearchLineEdit::keywordsEmpty, this,
          [&]() { listDirectory(); });
  connect(ui.searchLineEdit, &SearchLineEdit::keyPressed, this,
          &QtFinderWindow::onKeyPressed);
  connect(ui.quickfixWidget, &QuickfixWidget::keyPressed, this,
          &QtFinderWindow::onKeyPressed);
  connect(&rg_, &QProcess::readyRead, this, [&]() {
    QTextCodec *textCodec = QTextCodec::codecForName("UTF8");
    while (rg_.canReadLine()) {
      QString line = rg_.readLine();
      line = line.trimmed();
      ui.quickfixWidget->addItem(line);
    }
    ui.quickfixWidget->scrollToBottom();
  });
  connect(&rg_, &QProcess::readyReadStandardError, [&]() {
    QTextCodec *textCodec = QTextCodec::codecForName("UTF8");
    QString line = textCodec->fromUnicode(rg_.readAllStandardError());
    qDebug() << line;
  });
  connect(&rg_, static_cast<void (QProcess::*)(int)>(&QProcess::finished), this,
          [&](int exitCode) { ui.quickfixWidget->scrollToTop(); });
  ui.quickfixWidget->setUniformItemSizes(true);
}
QtFinderWindow::~QtFinderWindow() noexcept {}

void QtFinderWindow::show() {
  QWidget::show();
  listDirectory();
}

void QtFinderWindow::onSearchKeyWordsChanged(const QStringList &keywords,
                                             QtFinder::Cmd cmd) {
  switch (cmd) {
  case QtFinder::Cmd::kFd: {
    fdSearch(keywords);
    break;
  }
  case QtFinder::Cmd::kQuickfix: {
    // quickfixSearch(keywords);
    break;
  }
  case QtFinder::Cmd::kDirectoryChanged: {
    onDirectoryChanged(keywords.front());
    break;
  }
  default:
    break;
  }
  qDebug() << keywords;
}

void QtFinderWindow::onKeyPressed(Qt::Key key) {
  switch (key) {
  case Qt::Key_Enter: {
    onEnterKeyPressed();
    break;
  }
  case Qt::Key_Down: {
    ui.quickfixWidget->updateCurrentRow(QuickfixWidget::SelectOpt::kDown);
    break;
  }
  case Qt::Key_Up: {
    ui.quickfixWidget->updateCurrentRow(QuickfixWidget::SelectOpt::kUp);
    break;
  }
  default:
    break;
  }
}

void QtFinderWindow::onEnterKeyPressed() {
  if (ui.quickfixWidget->count() == 0) {
    return;
  }
  auto item = ui.quickfixWidget->currentItem();
  auto text = item->text();

  /// the text maybe a relative path or a absolute path,
  /// but we need convert relative path to absolute path
  QFileInfo fileInfo(text);
  if (!fileInfo.isAbsolute()) {
    fileInfo = directory_ + "/" + text;
  }
  if (!fileInfo.isDir()) {
    openDirectoryOfFile(fileInfo.absoluteFilePath());
    return;
  }
  directory_ = fileInfo.absoluteFilePath();

  listDirectory();
}

void QtFinderWindow::fdSearch(const QStringList &keywords) {
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
  QDir dir(directory);
  directory_ = directory;
  listDirectory();
}

void QtFinderWindow::listDirectory() {
  directory_ = directory_ == "~" ? QDir::homePath() : directory_;

  ui.quickfixWidget->clear();
  ui.promptLabel->setText(directory_);
  auto entrys = directoryEntryList(directory_);
  ui.quickfixWidget->addItems(entrys);
  ui.quickfixWidget->updateCurrentRow(QuickfixWidget::SelectOpt::kKeep);
}

void QtFinderWindow::openDirectoryOfFile(const QString &filePath) {
  fs::path absPath(filePath.toStdString());
  QString dir;
#ifdef Q_OS_WIN
  dir = absPath.string().c_str();
  QUrl url = QUrl::fromLocalFile(dir);
  QProcess explorer;
  QString cmd = "explorer.exe /select, " + url.toString();
  qDebug() << cmd;
  explorer.startDetached(cmd);
#else
  dir = absPath.parent_path().string().c_str();
  QUrl url = QUrl::fromLocalFile(dir);
  QDesktopServices::openUrl(url);
#endif
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
  QDir::Filters filters = QDir::Dirs | QDir::Files;
  filters |= dir.isRoot() ? QDir::NoDotAndDotDot : QDir::NoDot;
  return dir.entryList(filters);
}
