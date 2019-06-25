#ifndef QTFINDERAPP_H
#define QTFINDERAPP_H

#include <QObject>

class QtFinderAppPrivate;
class QtFinderApp : public QObject {
  Q_OBJECT
public:
  QtFinderApp(QObject *parent = nullptr);
  virtual ~QtFinderApp() noexcept;
  void run();

signals:

private:
  Q_DECLARE_PRIVATE(QtFinderApp);
  Q_DISABLE_COPY(QtFinderApp)

  QScopedPointer<QtFinderAppPrivate> d_ptr;
};

#endif /* QTFINDERAPP_H */
