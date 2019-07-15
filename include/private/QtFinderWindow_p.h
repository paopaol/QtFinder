
#ifndef QTFINDERWINDOW_PRIVATE_H
#define QTFINDERWINDOW_PRIVATE_H

#include <QtFinderCmd.h>
#include <ui_QtFinderWindow.h>

#include <QProcess>

class QtFinderWindowPrivate final : public QWidget {
  Q_OBJECT
public:
  QtFinderWindowPrivate(QWidget *parent = nullptr);
  QtFinderWindowPrivate(const QtFinderWindowPrivate &other) = delete;
  QtFinderWindowPrivate &operator=(const QtFinderWindowPrivate &other) = delete;

  void setSearchKeywords(const QtFinder::Cmd cmd, const QString &keywords);
  void setFdCmdTriggerDelay(int delayMs);
  void clearSearchKeywords();
  QString currentDirectory() const;
  void openCandidateAsPath(int index);
  void openCandidateAsDirectory(int index);

  virtual ~QtFinderWindowPrivate() noexcept;

signals:
  void searchKeywordsChanged(const QStringList &keywords, QtFinder::Cmd cmd);
  void keywordsEmpty();
  void openPathChanged(const QString &path);
  void openDirectoryChanged(const QString &directory);
  void candidateEmpty();

protected:
  Ui::Widget ui;
  QString directory_{"~"};

  friend class QtFinderWindow;
};

#endif /* QTFINDERWINDOW_H */
