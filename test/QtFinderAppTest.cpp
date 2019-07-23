#include <QSignalSpy>
#include <QString>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <QTest>
#include <QtFinderApp.h>

class QtFinderAppTest : public QObject {
  Q_OBJECT
private slots:
  void candidateIsDirectory_selectCandidateAsPath_ScanThePath();
};

class MockFinderTool final : public AbstractQtFinderTool {
  Q_OBJECT
public:
  MockFinderTool() {}
  ~MockFinderTool() {}
  void startSearchOnDirectory(const QString &absolutePath,
                              const QStringList &keywords) override {
    emit candidateReady("ssss");
  }
  void stop() override {}
};
class MockFinderWindow final : public AbstractQtFinderWindow {
  Q_OBJECT
public:
  void setSearchKeywords(const QtFinder::Cmd cmd,
                         const QString &keywords) override {
    emit searchKeywordsChanged(cmd, QStringList() << keywords);
  }
  void setFdCmdTriggerDelay(int delayMs) override {}
  void clearSearchKeywords() override {}
  QString currentDirectory() const override { return ""; }
  void setCurrentDirectory(const QString &absolutePath) override {}
  void selectCandidateAsPath(int row) override {
    emit selectedPathChanged("/mock/test/directory");
  }
  void selectCandidateAsDirectory(int row) override {
    emit selectedDirectoryChanged("/mock/test/directory");
  }
  void addCandidate(const QString &candidate) override {
    QCOMPARE(candidate, QString("xxx"));
  }
  int candidateSize() const override { return 1; }
};

class MockDesktopService final : public AbstractDesktopService {
public:
  MockDesktopService() {}
  virtual ~MockDesktopService() {}
  void openDirectoryOfFile(const QString &path) override {
    QCOMPARE(path, QString("/mock/test/directory"));
  }
  void openDirectoryOrFile(const QString &path) override {
    QCOMPARE(path, QString("/mock/test/directory"));
  }
};

class MockFileSystemScanner final : public AbstractFileSystemScanner {
public:
  MockFileSystemScanner() {}
  virtual ~MockFileSystemScanner() {}
  QStringList scanDirectory() override {
    QStringList entryList;
    entryList << "/mock/test/directory";
    return entryList;
  }
};

void QtFinderAppTest::candidateIsDirectory_selectCandidateAsPath_ScanThePath() {
  QtFinderApp app;
  QScopedPointer<AbstractQtFinderTool> finderTool(new MockFinderTool);
  QScopedPointer<AbstractQtFinderWindow> finderWindow(new MockFinderWindow);
  QScopedPointer<AbstractDesktopService> desktopService(new MockDesktopService);
  QScopedPointer<AbstractFileSystemScanner> fileSystemScanner(
      new MockFileSystemScanner);

  app.setFinderTool(finderTool);
  app.setFinderWindow(finderWindow);
  app.setDesktopService(desktopService);
  app.setFileSystemScanner(fileSystemScanner);

  app.startSearch(QtFinder::Cmd::kFd, "keywords");
  QTest::keyPress(&app, Qt::Key_Enter, Qt::NoModifier, 1000);
}

QTEST_MAIN(QtFinderAppTest)
#include "QtFinderAppTest.moc"
