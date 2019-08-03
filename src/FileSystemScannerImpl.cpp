#include "FileSystemScannerImpl.h"
#include <QDir>

FileSystemScannerImpl::FileSystemScannerImpl(QObject *parent)
    : AbstractFileSystemScanner(parent) {}

FileSystemScannerImpl::~FileSystemScannerImpl() {}

QStringList FileSystemScannerImpl::scanDirectory(const QString &directory) {
  QDir dir(directory);
  QDir::Filters filters = QDir::Dirs | QDir::Files | QDir::Hidden;
  filters |= dir.isRoot() ? QDir::NoDotAndDotDot : QDir::NoDot;
  return dir.entryList(filters);
}
