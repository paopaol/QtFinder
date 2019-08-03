#ifndef QTFINDERAPP_P_H
#define QTFINDERAPP_P_H

#include <QHBoxLayout>
#include <QObject>
#include <QScopedPointer>
#include <QtFinderApp.h>
#include <mutex>
#include <qtfindercmd.h>

class AbstractQtFinderWindow;
class AbstractQtFinderTool;
class AbstractFileSystemScanner;
class AbstractDesktopService;
class QtFinderAppPrivate final : public QObject {
  Q_OBJECT
public:
  QtFinderAppPrivate(QtFinderApp *app) : QObject(app) {}
  virtual ~QtFinderAppPrivate() {}

private:
  void prepare();
  void onSearchKeywordsChanged(QtFinder::Cmd cmd, const QStringList &keywords);
  void onSelectedFileChanged(const QString &file);

  Q_DISABLE_COPY(QtFinderAppPrivate);

  QScopedPointer<AbstractQtFinderWindow> win_;
  QScopedPointer<AbstractQtFinderTool> tool_;
  QScopedPointer<AbstractFileSystemScanner> fileSystemScanner_;
  QScopedPointer<AbstractDesktopService> desktopService_;
  std::once_flag onceFlag_;
  QHBoxLayout layout_;

  friend class QtFinderApp;
};

#endif /* QTFINDERAPP_P_H */
