#ifndef QTFINDERWINDOW_H
#define QTFINDERWINDOW_H

#include <QMetaType>
#include <QWidget>
#include <QtFinderCmd.h>

#include <QProcess>

class QtFinderWindowPrivate;
class QtFinderWindow : public QWidget {
  Q_OBJECT
  Q_DECLARE_PRIVATE(QtFinderWindow)
public:
  QtFinderWindow(QWidget *parent = nullptr);
  QtFinderWindow(const QtFinderWindow &other) = delete;
  QtFinderWindow &operator=(const QtFinderWindow &other) = delete;
  virtual ~QtFinderWindow() noexcept;

  // void show();

private slots:
  // void onSearchKeyWordsChanged(const QStringList &keyWords, QtFinder::Cmd cmd);
  // void onKeyPressed(Qt::Key key);
  // void onEnterKeyPressed();
  // void onCtrlEnterPressed();
  // void fdSearch(const QStringList &keyWords);
  // void onDirectoryChanged(const QString &directory);
  // void showDirectory();
  // void showUpDirectory();
  // void openDirectoryOfFile(const QString &path);
  // void openDirectoryOrFile(const QString &path);

protected:
  QProcess fd_;
  QScopedPointer<QtFinderWindowPrivate> d_ptr;
};

#endif /* QTFINDERWINDOW_H */
