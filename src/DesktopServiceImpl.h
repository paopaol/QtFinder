#ifndef __DESKTOPSERVICEIMPL_H_
#define __DESKTOPSERVICEIMPL_H_

#include <QtFinderAbstractComponents.h>

class DesktopServiceImpl : public AbstractDesktopService {
  Q_OBJECT
public:
  DesktopServiceImpl(QObject *parent = nullptr);
  virtual ~DesktopServiceImpl();
  void openWithFileManager(const QString &path) override;
  void openFile(const QString &path) override;
};

#endif // __DESKTOPSERVICEIMPL_H_
