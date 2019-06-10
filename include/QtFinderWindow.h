#ifndef QTFINDERWINDOW_H
#define QTFINDERWINDOW_H

#include <QtFinderCmd.h>
#include <ui_main.h>

#include <QProcess>

class QtFinderWindow : public QWidget {
public:
  QtFinderWindow(QWidget *parent = nullptr);
  QtFinderWindow(const QtFinderWindow &other) = delete;
  QtFinderWindow &operator=(const QtFinderWindow &other) = delete;
  virtual ~QtFinderWindow() noexcept;

  void show();

private slots:
  void onSearchKeyWordsChanged(const QStringList &keyWords, QtFinder::Cmd cmd);
  void onKeyPressed(Qt::Key key);
  void onEnterKeyPressed();
  void fdSearch(const QStringList &keyWords);
  void onDirectoryChanged(const QString &directory);
  void listDirectory();
  void openDirectoryOfFile(const QString &path);

private:
  Ui::Widget ui;
  QProcess fd_;
  QProcess rg_;

  QString directory_{"~"};
};

#endif /* QTFINDERWINDOW_H */
