#ifndef QTFINDERWINDOWIMPL_H
#define QTFINDERWINDOWIMPL_H

#include <QtFinderAbstractComponents.h>
#include <QtFinderWindow.h>

class QtFinderWindowImpl : public AbstractQtFinderWindow {
  Q_OBJECT
public:
  QtFinderWindowImpl(QWidget *parent = nullptr);
  ~QtFinderWindowImpl();

  void setSearchKeywords(QtFinder::Cmd cmd, const QString &keywords) override;
  void setFdCmdTriggerDelay(int delayMs) override;
  void clearSearchKeywords() override;
  QString currentDirectory() const override;
  void setCurrentDirectory(const QString &absolutePath) override;
  void selectCandidateAsFile(int row) override;
  void selectCandidateAsDirectory(int row) override;
  void addCandidates(const QStringList &candidates) override;
  void setCandidates(const QStringList &candidates) override;
  int candidateSize() const override;
  void focusRow(int row) override;

private:
  QtFinderWindow win_;
  QHBoxLayout layout_;
};

#endif /* QTFINDERWINDOWIMPL_H */
