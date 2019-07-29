#ifndef QTFINDERAPP_H
#define QTFINDERAPP_H

#include <QObject>
#include <QWidget>
#include <QtFinderAbstractComponents.h>


class QtFinderAppPrivate;
class QtFinderApp : public QWidget {
  Q_OBJECT

public:
  QtFinderApp(QWidget *parent = nullptr);
  virtual ~QtFinderApp() noexcept;

  void setFinderTool(AbstractQtFinderToolPtr &finderTool);
  void setFinderWindow(AbstractQtFinderWindowPtr &finderWindow);
  void setDesktopService(AbstractDesktopServicePtr &desktopService);
  void setFileSystemScanner(AbstractFileSystemScannerPtr &fileSystemScanner);

  void run();

protected:
  virtual void keyPressEvent(QKeyEvent *e) override;

private:
  Q_DECLARE_PRIVATE(QtFinderApp);

  QScopedPointer<QtFinderAppPrivate> d_ptr;
};

#endif /* QTFINDERAPP_H */
