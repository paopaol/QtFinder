#include <QSignalSpy>
#include <QString>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <QTest>
#include <QtFinderApp.h>
#include <gmock/gmock.h>

using namespace ::testing;

class QtFinderAppTest : public QObject {
  Q_OBJECT
private slots:
  void initTestCase() {
    int argc = 0;
    char *argv[] = {""};
    ::testing::InitGoogleMock(&argc, argv);
  }
  void candidateIsDirectory_selectCandidateAsFile_ScanThePath();
};

class MockFinderTool final : public AbstractQtFinderTool {
  Q_OBJECT
public:
  MockFinderTool() {}
  ~MockFinderTool() {}
  MOCK_METHOD2(startSearchOnDirectory,
               void(const QString &absolutePath, const QStringList &keywords));
  void stop() override {}
};
class MockFinderWindow final : public AbstractQtFinderWindow {
  Q_OBJECT
public:
  MockFinderWindow() : AbstractQtFinderWindow() {}
  ~MockFinderWindow() {}
  void setSearchKeywords(const QtFinder::Cmd cmd,
                         const QString &keywords) override {
    emit searchKeywordsChanged(cmd, QStringList() << keywords);
  }
  void setFdCmdTriggerDelay(int delayMs) override {}
  void clearSearchKeywords() override {}
  QString currentDirectory() const override { return ""; }
  void setCurrentDirectory(const QString &absolutePath) override {}
  void selectCandidateAsFile(int row) override {
    emit selectedFilechanged("/mock/test/directory");
  }
  MOCK_METHOD(void, selectCandidateAsDirectory, (int), (override));
  MOCK_METHOD1(addCandidate, void(const QString &candidate));
  MOCK_METHOD(int, candidateSize, (), (const));
  MOCK_METHOD(void, keyPressEvent, (QKeyEvent *), (override));
  // protected:
  //   void keyPressEvent(QKeyEvent *e) {
  //	QWidget::keyPressEvent(e);
  // }
};

class MockDesktopService final : public AbstractDesktopService {
public:
  MockDesktopService() {}
  virtual ~MockDesktopService() {}
  MOCK_METHOD(void, openWithFileManager, (const QString &path), (override));
  MOCK_METHOD(void, openFile, (const QString &path), (override));
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

ACTION_P2(emitCandidateReady, tool, candidate) {
  tool->candidateReady(candidate);
}

ACTION_P2(emitSelectedFileChanged, win, dirOrFile) {
  win->selectedFilechanged(dirOrFile);
}

ACTION_P2(validateOpenDirectoryOrFile, p1, p2) { QCOMPARE(p1, p2); }

void QtFinderAppTest::candidateIsDirectory_selectCandidateAsFile_ScanThePath() {
  QtFinderApp app;
  app.setWindowTitle("test");
  const char *searchKeywords = "foo bar directory";
  const char *searchResult = "/tmp/foo/bar/directory";

  QScopedPointer<AbstractQtFinderTool> finderTool(new MockFinderTool);
  QScopedPointer<AbstractQtFinderWindow> finderWindow(new MockFinderWindow);
  QScopedPointer<AbstractDesktopService> desktopService(new MockDesktopService);
  QScopedPointer<AbstractFileSystemScanner> fileSystemScanner(
      new MockFileSystemScanner);

  auto tool = static_cast<MockFinderTool *>(finderTool.data());
  EXPECT_CALL(*tool, startSearchOnDirectory(::testing::_, ::testing::_))
      .WillOnce(emitCandidateReady(tool, searchResult));
  auto win = static_cast<MockFinderWindow *>(finderWindow.data());
  EXPECT_CALL(*win, addCandidate(::testing::_));
  EXPECT_CALL(*win, keyPressEvent(::testing::_))
      .WillOnce(emitSelectedFileChanged(win, searchResult));

  auto desktop = static_cast<MockDesktopService *>(desktopService.data());
  EXPECT_CALL(*desktop, openFile(::testing::_))
      .WillOnce(validateOpenDirectoryOrFile(searchResult, searchResult));

  app.setFinderTool(finderTool);
  app.setFinderWindow(finderWindow);
  app.setDesktopService(desktopService);
  app.setFileSystemScanner(fileSystemScanner);

  app.startSearch(QtFinder::Cmd::kFd, searchKeywords);
  QTest::keyPress(win, Qt::Key_Enter, Qt::NoModifier);
}

QTEST_MAIN(QtFinderAppTest)
#include "QtFinderAppTest.moc"
