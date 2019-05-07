#ifndef QTFINDERWINDOW_H
#define QTFINDERWINDOW_H

#include <ui_main.h>

class QtFinderWindow : public QWidget {
public:
  QtFinderWindow(QWidget *parent = nullptr);
  QtFinderWindow(const QtFinderWindow &other) = delete;
  QtFinderWindow &operator=(const QtFinderWindow &other) = delete;
  virtual ~QtFinderWindow() noexcept;

private slots:
  void onSearchKeyWordsChanged(const QStringList &keyWords);

private:
  Ui::Widget uiWidget;
};

#endif /* QTFINDERWINDOW_H */
