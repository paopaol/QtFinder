#include "DesktopServiceImpl.h"
#include <QDesktopServices>
#include <QProcess>
#include <QUrl>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

DesktopServiceImpl::DesktopServiceImpl(QObject *parent)
    : AbstractDesktopService(parent) {}

DesktopServiceImpl::~DesktopServiceImpl() {}

void DesktopServiceImpl::openWithFileManager(const QString &path) {
  fs::path absPath(path.toStdString());
  QString dir;
#ifdef Q_OS_WIN
  dir = absPath.string().c_str();
  QUrl url = QUrl::fromLocalFile(dir);
  QProcess explorer;
  QString cmd = "explorer.exe /select, " + url.toString();
  explorer.startDetached(cmd);
#else
  dir = absPath.parent_path().string().c_str();
  QUrl url = QUrl::fromLocalFile(dir);
  QDesktopServices::openUrl(url);
#endif
}

void DesktopServiceImpl::openFile(const QString &path) {
  QUrl url = QUrl::fromLocalFile(path);
  QDesktopServices::openUrl(url);
}
