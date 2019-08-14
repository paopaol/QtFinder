#ifndef __QTFINDERABSTRACTCOMPONENTS_H_
#define __QTFINDERABSTRACTCOMPONENTS_H_

#include <QWidget>
#include <qtfindercmd.h>

class AbstractQtFinderTool : public QObject {
  Q_OBJECT
public:
  explicit AbstractQtFinderTool(QObject *parent = nullptr) {}
  virtual ~AbstractQtFinderTool() {}
  virtual void startSearchOnDirectory(const QString &absolutePath,
                                      const QStringList &keywords) = 0;
  virtual void stop() = 0;
signals:
  void candidatesReady(const QStringList &candidates);
  void started();
  void stopped();
};

using AbstractQtFinderToolPtr = QScopedPointer<AbstractQtFinderTool>;

class AbstractQtFinderWindow : public QWidget {
  Q_OBJECT
public:
  explicit AbstractQtFinderWindow(QWidget *parent = nullptr)
      : QWidget(parent) {}
  virtual ~AbstractQtFinderWindow() {}
  virtual void setSearchKeywords(QtFinder::Cmd cmd,
                                 const QString &keywords) = 0;
  virtual void setFdCmdTriggerDelay(int delayMs) = 0;
  virtual void clearSearchKeywords() = 0;
  virtual QString currentDirectory() const = 0;
  virtual void setCurrentDirectory(const QString &absolutePath) = 0;
  virtual void selectCandidateAsFile(int row) = 0;
  virtual void selectCandidateAsDirectory(int row) = 0;
  virtual void addCandidates(const QStringList &candidates) = 0;
  virtual void setCandidates(const QStringList &candidates) = 0;
  virtual int candidateSize() const = 0;
  virtual void focusRow(int row) = 0;

signals:
  void searchKeywordsChanged(QtFinder::Cmd cmd, const QStringList &keywords);
  void keywordsEmpty();
  void selectedFilechanged(const QString &path);
  void selectedDirectoryChanged(const QString &directory);
  void candidateEmpty();
};
using AbstractQtFinderWindowPtr = QScopedPointer<AbstractQtFinderWindow>;

class AbstractDesktopService : public QObject {
  Q_OBJECT
public:
  explicit AbstractDesktopService(QObject *parent = nullptr) {}
  virtual ~AbstractDesktopService() {}
  virtual void openWithFileManager(const QString &path) = 0;
  virtual void openFile(const QString &path) = 0;
};
using AbstractDesktopServicePtr = QScopedPointer<AbstractDesktopService>;

class AbstractFileSystemScanner : public QObject {
  Q_OBJECT
public:
  explicit AbstractFileSystemScanner(QObject *parent = nullptr){};
  virtual ~AbstractFileSystemScanner() {}
  virtual QStringList scanDirectory(const QString &directory) = 0;
};
using AbstractFileSystemScannerPtr = QScopedPointer<AbstractFileSystemScanner>;

#endif // __QTFINDERABSTRACTCOMPONENTS_H_
