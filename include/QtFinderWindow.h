#ifndef QTFINDERWINDOW_H
#define QTFINDERWINDOW_H

#include <ui_main.h>

#include <QProcess>
#include <QTimer>

class QtFinderWindow : public QWidget {
public:
  QtFinderWindow(QWidget *parent = nullptr);
  QtFinderWindow(const QtFinderWindow &other) = delete;
  QtFinderWindow &operator=(const QtFinderWindow &other) = delete;
  virtual ~QtFinderWindow() noexcept;

private slots:
  void onSearchKeyWordsChanged(const QStringList &keyWords);
  void search(const QStringList &keyWords, bool depth = false);
  void onDirectoryChanged(const QString &directory);

private:
  Ui::Widget ui;
  QProcess fd_;
  QProcess rg_;

  /**@brief Delay in triggering search behavior after entering a keyword
   */
  int searchDelay_{400};
  QTimer delayTimer_;
  QStringList keyWords_;
  QString directory_{"~"};
};

#endif /* QTFINDERWINDOW_H */
