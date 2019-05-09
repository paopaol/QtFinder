#ifndef QTFINDERWINDOW_H
#define QTFINDERWINDOW_H

#include <ui_main.h>

#include <QProcess>

class QtFinderWindow : public QWidget {
public:
  QtFinderWindow(QWidget *parent = nullptr);
  QtFinderWindow(const QtFinderWindow &other) = delete;
  QtFinderWindow &operator=(const QtFinderWindow &other) = delete;
  virtual ~QtFinderWindow() noexcept;

private slots:
  void onSearchKeyWordsChanged(const QStringList &keyWords);
  void search(const QStringList &keyWords);

private:
  Ui::Widget uiWidget;
  QProcess fd_;
  QProcess rg_;
};

#endif /* QTFINDERWINDOW_H */
