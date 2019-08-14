#ifndef __QTFINDERTOOLIMPL_H_
#define __QTFINDERTOOLIMPL_H_

#include <QtFinderAbstractComponents.h>
#include <QtFinderTool.h>

class QtFinderToolImpl : public AbstractQtFinderTool {
  Q_OBJECT
public:
  QtFinderToolImpl(QObject *parent = nullptr);
  virtual ~QtFinderToolImpl() {}
  virtual void startSearchOnDirectory(const QString &absolutePath,
                                      const QStringList &keywords) override;
  virtual void stop() override;

private:
  QScopedPointer<QtFinderTool> tool_;
};
#endif // __QTFINDERTOOLIMPL_H_
