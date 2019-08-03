#include <QSignalSpy>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <QTest>
#include <QtFinderWindow.h>

class QtFinderWindowTest : public QObject {
  Q_OBJECT
private slots:
  void defaultDirectoryIsHome();
  void currentDirectoryIsHome_setCurrentDirectory_Works();
  void keywordsIsEmpty_setFdKeywords_gotSignalFdKeywordsChanged();
  void keywordsNotEmpty_clearKeywords_gotSignalKeywordsEmpty();
  void candidateIsEmpty_selectCandidate_gotSignalCandidateEmpty();
  void candidateisEmpty_addTwoCandidates_sizeIsTwo();
  void hasSomeCandidates_selectCandidate_gotSelectAsFileSignal();
  void hasSomeCandidates_moveDownUp_Works();
};

void QtFinderWindowTest::defaultDirectoryIsHome() {
  QtFinderWindow win;
  QCOMPARE(win.currentDirectory(), QDir::homePath());
}

void QtFinderWindowTest::currentDirectoryIsHome_setCurrentDirectory_Works() {
  QtFinderWindow win;

  QTemporaryDir dir;

  win.setCurrentDirectory(dir.filePath("test"));

  QCOMPARE(win.currentDirectory(), dir.filePath("test"));
}

void QtFinderWindowTest::
    keywordsIsEmpty_setFdKeywords_gotSignalFdKeywordsChanged() {
  QtFinderWindow win;
  win.show();

  QSignalSpy spy(&win, &QtFinderWindow::searchKeywordsChanged);

  win.setFdCmdTriggerDelay(3000);
  win.setSearchKeywords(QtFinder::Cmd::kFd, "search keywords");

  spy.wait(5000);

  QCOMPARE(spy.count(), 1);
  auto arguments = spy.takeFirst();
  auto cmd = qvariant_cast<QtFinder::Cmd>(arguments.at(0));
  auto keywords = arguments.at(1).toStringList();
  QCOMPARE(keywords, QStringList() << "search"
                                   << "keywords");
  QCOMPARE(cmd, QtFinder::Cmd::kFd);
}

void QtFinderWindowTest::
    keywordsNotEmpty_clearKeywords_gotSignalKeywordsEmpty() {
  QtFinderWindow win;

  QSignalSpy spy(&win, &QtFinderWindow::keywordsEmpty);
  win.setSearchKeywords(QtFinder::Cmd::kQuickfix, "keywords");
  win.clearSearchKeywords();
  QCOMPARE(spy.count(), 1);
}

void QtFinderWindowTest::
    candidateIsEmpty_selectCandidate_gotSignalCandidateEmpty() {
  QtFinderWindow win;
  QSignalSpy spy(&win, &QtFinderWindow::candidateEmpty);

  win.selectCandidateAsFile(0);
  QCOMPARE(spy.count(), 1);
}

void QtFinderWindowTest::candidateisEmpty_addTwoCandidates_sizeIsTwo() {
  QtFinderWindow win;

  win.addCandidate("candidate_1");
  win.addCandidate("candidate_2");
  QCOMPARE(win.candidateSize(), 2);
}

void QtFinderWindowTest::
    hasSomeCandidates_selectCandidate_gotSelectAsFileSignal() {
  QtFinderWindow win;

  win.show();

  QSignalSpy spy(&win, &QtFinderWindow::selectedDirectoryChanged);

  QTemporaryFile f1("f1");
  QTemporaryFile f2("f2");

  f1.open();
  f2.open();

  win.addCandidate(f1.fileName());
  win.addCandidate(f2.fileName());

  // win.selectCandidateAsFile(1);
  QTest::keyPress(&win, Qt::Key_Enter, Qt::ControlModifier);
  spy.wait(3000);

  QCOMPARE(spy.count(), 1);
}

void QtFinderWindowTest::hasSomeCandidates_moveDownUp_Works() {
  QtFinderWindow win;

  win.show();

  QSignalSpy spy(&win, &QtFinderWindow::selectedDirectoryChanged);

  QTemporaryFile f1("f1");
  QTemporaryFile f2("f2");

  f1.open();
  f2.open();

  win.addCandidate(f1.fileName());
  win.addCandidate(f2.fileName());

  // win.selectCandidateAsFile(1);
  QTest::keyPress(&win, Qt::Key_Down, Qt::NoModifier, 1000);
  QTest::keyPress(&win, Qt::Key_Down, Qt::NoModifier, 1000);
  QTest::keyPress(&win, Qt::Key_Up, Qt::NoModifier, 1000);
  QTest::keyPress(&win, Qt::Key_Up, Qt::NoModifier, 1000);
  QTest::keyPress(&win, Qt::Key_Enter, Qt::ControlModifier, 1000);
  spy.wait(7000);

  QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(QtFinderWindowTest)
#include "QtFinderWindowTest.moc"
