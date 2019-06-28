
#ifndef QTFINDERWINDOW_PRIVATE_H
#define QTFINDERWINDOW_PRIVATE_H

#include <QtFinderCmd.h>
#include <ui_QtFinderWindow.h>

#include <QProcess>

class QtFinderWindowPrivate : public QWidget {
public:
  QtFinderWindowPrivate(QWidget *parent = nullptr);
  QtFinderWindowPrivate(const QtFinderWindowPrivate &other) = delete;
  QtFinderWindowPrivate &operator=(const QtFinderWindowPrivate &other) = delete;
  virtual ~QtFinderWindowPrivate() noexcept;

protected:
  Ui::Widget ui;
  QString directory_{"~"};
};

#endif /* QTFINDERWINDOW_H */
