#include "QtFinderTool.h"
#include <QDir>
#include <QProcess>
#include <QTextCodec>
#include <QTextStream>

static QString fdPattern(const QStringList &keywords);

QtFinderTool::QtFinderTool(QObject *parent)
    : QObject(parent), fd_(new QProcess) {
  connect(fd_.data(), &QProcess::readyRead, this, [&]() {
    QTextCodec *textCodec = QTextCodec::codecForName("UTF8");
    while (fd_->canReadLine()) {
      QString line = fd_->readLine();
      line = line.trimmed();
      line = QDir::fromNativeSeparators(line);
      emit candidateReady(line);
    }
  });
}

QtFinderTool::~QtFinderTool() { stop(); }

void QtFinderTool::startSearchOnDirectory(const QString &absolutePath,
                                          const QStringList &keywords) {
  stop();

  QStringList fdArgs;
  fdArgs << "-p" << fdPattern(keywords) << absolutePath;

  fd_->start("fd", fdArgs);
  fd_->waitForStarted();
}

void QtFinderTool::stop() {
  if (fd_->state() == QProcess::ProcessState::Starting) {
    fd_->waitForStarted();
  }

  if (fd_->state() == QProcess::ProcessState::Running) {
    fd_->kill();
    fd_->waitForFinished();
  }
}

static QString fdPattern(const QStringList &keywords) {
  QString pattern;
  QTextStream st(&pattern);
  for (auto &key : keywords) {
    st << key << ".*";
  }
  return pattern;
}
