#include <QSignalSpy>
#include <QString>
#include <QStringList>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <QTest>
#include <QtFinderApp.h>
#include <gmock/gmock.h>

using namespace ::testing;

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
  MOCK_METHOD(void, setSearchKeywords, (QtFinder::Cmd, const QString &keywords),
              (override));
  void setFdCmdTriggerDelay(int delayMs) override {}
  void clearSearchKeywords() override {}
  QString currentDirectory() const override { return ""; }
  MOCK_METHOD(void, setCurrentDirectory, (const QString &absolutePath),
              (override));
  // void selectCandidateAsFile(int row) override {
  //   emit selectedFilechanged("/mock/test/directory");
  // }
  MOCK_METHOD(void, selectCandidateAsFile, (int row), (override));
  MOCK_METHOD(void, selectCandidateAsDirectory, (int), (override));
  MOCK_METHOD(void, addCandidate, (const QString &candidate), (override));
  MOCK_METHOD(void, setCandidates, (const QStringList &candidates), (override));
  MOCK_METHOD(int, candidateSize, (), (const));
  MOCK_METHOD(void, keyPressEvent, (QKeyEvent *), (override));
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
  MOCK_METHOD(QStringList, scanDirectory, (const QString &dir), (override));
};

class QtFinderAppTest : public QObject {
  Q_OBJECT
private slots:
  void initTestCase() {
    int argc = 0;
    char *argv[] = {""};
    ::testing::InitGoogleMock(&argc, argv);
  }

  void init() {
    app_.reset(new QtFinderApp);

    finderTool_.reset(new MockFinderTool);
    finderWindow_.reset(new MockFinderWindow);
    desktopService_.reset(new MockDesktopService);
    fileSystemScanner_.reset(new MockFileSystemScanner);
    tool_ = static_cast<MockFinderTool *>(finderTool_.data());
    win_ = static_cast<MockFinderWindow *>(finderWindow_.data());
    desktop_ = static_cast<MockDesktopService *>(desktopService_.data());
    scanner_ = static_cast<MockFileSystemScanner *>(fileSystemScanner_.data());
  }

  void cleanup() {
    app_.reset(nullptr);
    finderTool_.reset(nullptr);
    finderWindow_.reset(nullptr);
    desktopService_.reset(nullptr);
    fileSystemScanner_.reset(nullptr);
    tool_ = nullptr;
    win_ = nullptr;
    desktop_ = nullptr;
    scanner_ = nullptr;
  }

  void candidateIsFile_openFile_Works();
  // void candidateIsDirectory_scanDirectory_Works();

private:
  QScopedPointer<QtFinderApp> app_;
  QScopedPointer<AbstractQtFinderTool> finderTool_;
  MockFinderTool *tool_;
  QScopedPointer<AbstractQtFinderWindow> finderWindow_;
  MockFinderWindow *win_;
  QScopedPointer<AbstractDesktopService> desktopService_;
  MockDesktopService *desktop_;
  QScopedPointer<AbstractFileSystemScanner> fileSystemScanner_;
  MockFileSystemScanner *scanner_;
};

ACTION_P2(emitCandidateReady, tool, candidate) {
  tool->candidateReady(candidate);
}

ACTION_P2(emitSelectedFileChanged, win, dirOrFile) {
  win->selectedFilechanged(dirOrFile);
}

ACTION_P2(validateOpenDirectoryOrFile, p1, p2) { QCOMPARE(p1, p2); }

void QtFinderAppTest::candidateIsFile_openFile_Works() {
  QTemporaryFile tempFile("foobarfile");
  tempFile.open();

  const char *searchKeywords = "foo bar file";

  QString searchResult = tempFile.fileName();

  EXPECT_CALL(*win_, setSearchKeywords(::testing::_, ::testing::_))
      .WillOnce(emitCandidateReady(tool_, searchResult));
  EXPECT_CALL(*win_, addCandidate(::testing::_));
  EXPECT_CALL(*win_, keyPressEvent(::testing::_))
      .WillOnce(emitSelectedFileChanged(win_, searchResult));

  EXPECT_CALL(*desktop_, openFile(::testing::_))
      .WillOnce(validateOpenDirectoryOrFile(searchResult, searchResult));

  app_->setFinderTool(finderTool_);
  app_->setFinderWindow(finderWindow_);
  app_->setDesktopService(desktopService_);
  app_->setFileSystemScanner(fileSystemScanner_);
  app_->run();

  win_->setSearchKeywords(QtFinder::Cmd::kFd, searchKeywords);
  QTest::keyPress(win_, Qt::Key_Enter, Qt::NoModifier);
}

QTEST_MAIN(QtFinderAppTest)
#include "QtFinderAppTest.moc"
