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
    QStringList candidates;
    while (fd_->canReadLine()) {
      QString line = fd_->readLine();
      line = line.trimmed();
      line = QDir::fromNativeSeparators(line);
      candidates << line;
    }
    emit candidatesReady(candidates);
  });
  connect(fd_.data(), &QProcess::started, this, &QtFinderTool::started);
  connect(fd_.data(), static_cast<void (QProcess::*)(int)>(&QProcess::finished),
          this, [&](int exitCode) { emit stopped(); });
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
