
#ifndef QTFINDERWINDOW_PRIVATE_H
#define QTFINDERWINDOW_PRIVATE_H

#include <QtFinderCmd.h>
#include <ui_QtFinderWindow.h>

#include <QProcess>

class QtFinderWindow final : public QWidget {
  Q_OBJECT
public:
  QtFinderWindow(QWidget *parent = nullptr);
  QtFinderWindow(const QtFinderWindow &other) = delete;
  QtFinderWindow &operator=(const QtFinderWindow &other) = delete;

  void setSearchKeywords(const QtFinder::Cmd cmd, const QString &keywords);
  void setFdCmdTriggerDelay(int delayMs);
  void clearSearchKeywords();
  QString currentDirectory() const;
  void setCurrentDirectory(const QString &absolutePath);
  void selectCandidateAsPath(int row);
  void selectCandidateAsDirectory(int row);
  void addCandidate(const QString &candidate);
  int candidateSize() const;

  virtual ~QtFinderWindow() noexcept;

protected:
  void keyPressEvent(QKeyEvent *) override;

signals:
  void searchKeywordsChanged(QtFinder::Cmd cmd, const QStringList &keywords);
  void keywordsEmpty();
  void selectedPathChanged(const QString &path);
  void selectedDirectoryChanged(const QString &directory);
  void candidateEmpty();
  void shortcutKeyPressed(Qt::Key key);

private:
  QString selectCandidate(int row);
  void shortcutKeyHandler(Qt::Key key);

  Ui::Widget ui;
  QString directory_{"~"};

  friend class QtFinderWindow;
};

#endif /* QTFINDERWINDOW_H */
