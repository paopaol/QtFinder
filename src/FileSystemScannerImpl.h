#ifndef __FILESYSTEMSCANNERIMPL_H_
#define __FILESYSTEMSCANNERIMPL_H_
#include <QtFinderAbstractComponents.h>

class FileSystemScannerImpl : public AbstractFileSystemScanner {
public:
  explicit FileSystemScannerImpl(QObject *parent = nullptr);
  virtual ~FileSystemScannerImpl();
  QStringList scanDirectory(const QString &directory) override;
};

#endif // __FILESYSTEMSCANNERIMPL_H_
