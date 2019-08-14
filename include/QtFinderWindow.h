
#ifndef QTFINDERWINDOW_PRIVATE_H
#define QTFINDERWINDOW_PRIVATE_H

#include <QtFinderCmd.h>
#include <ui_QtFinderWindow.h>

#include <QProcess>

class QtFinderWindow final : public QWidget {
  Q_OBJECT
public:
  QtFinderWindow(QWidget *parent = nullptr);
  virtual ~QtFinderWindow() noexcept;
  QtFinderWindow(const QtFinderWindow &other) = delete;
  QtFinderWindow &operator=(const QtFinderWindow &other) = delete;

  void setSearchKeywords(const QtFinder::Cmd cmd, const QString &keywords);
  void setFdCmdTriggerDelay(int delayMs);
  void clearSearchKeywords();
  QString currentDirectory() const;
  void setCurrentDirectory(const QString &absolutePath);
  void selectCandidateAsFile(int row);
  void selectCandidateAsDirectory(int row);
  void addCandidates(const QStringList &candidates);
  void setCandidates(const QStringList &candidates);
  int candidateSize() const;
  void focusRow(int row);

protected:
  void keyPressEvent(QKeyEvent *) override;

signals:
  void searchKeywordsChanged(QtFinder::Cmd cmd, const QStringList &keywords);
  void keywordsEmpty();
  void selectedFilechanged(const QString &path);
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
