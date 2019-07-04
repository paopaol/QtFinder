#ifndef QTFINDERAPP_P_H
#define QTFINDERAPP_P_H

#include <QtFinderWindow.h>

class QtFinderAppPrivate : public QObject {
  Q_OBJECT
public:
  QtFinderAppPrivate(QObject *parent = nullptr);
  virtual ~QtFinderAppPrivate() noexcept;


private:
  Q_DISABLE_COPY(QtFinderAppPrivate)

  QtFinderWindow window_;
  QtFinder finder_;
};

#endif /* QTFINDERAPP_P_H */
