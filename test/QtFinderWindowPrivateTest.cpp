#include <QSignalSpy>
#include <QTemporaryFile>
#include <QTest>
#include <private/QtFinderWindow_p.h>

class QtFinderWindowPrivateTest : public QObject {
  Q_OBJECT
private slots:
  void defaultDirectoryIsHome();
  void keywordsIsEmpty_setFdKeywords_gotSignalFdKeywordsChanged();
  void keywordsNotEmpty_clearKeywords_gotSignalKeywordsEmpty();
  void candidateIsEmpty_selectCandidate_gotSignalCandidateEmpty();
  void candidateisEmpty_addTwoCandidates_sizeIsTwo();
  void hasSomeCandidates_selectCandidate_gotSelectAsPathSignal();
};

void QtFinderWindowPrivateTest::defaultDirectoryIsHome() {
  QtFinderWindowPrivate win;
  QCOMPARE(win.currentDirectory(), QString("~"));
}

void QtFinderWindowPrivateTest::
    keywordsIsEmpty_setFdKeywords_gotSignalFdKeywordsChanged() {
  QtFinderWindowPrivate win;
  win.show();

  QSignalSpy spy(&win, &QtFinderWindowPrivate::searchKeywordsChanged);

  win.setFdCmdTriggerDelay(3000);
  win.setSearchKeywords(QtFinder::Cmd::kFd, "search keywords");

  spy.wait(5000);

  QCOMPARE(spy.count(), 1);
  auto arguments = spy.takeFirst();
  auto keywords = arguments.at(0).toStringList();
  auto cmd = qvariant_cast<QtFinder::Cmd>(arguments.at(1));
  QCOMPARE(keywords, QStringList() << "search"
                                   << "keywords");
  QCOMPARE(cmd, QtFinder::Cmd::kFd);
}

void QtFinderWindowPrivateTest::
    keywordsNotEmpty_clearKeywords_gotSignalKeywordsEmpty() {
  QtFinderWindowPrivate win;

  QSignalSpy spy(&win, &QtFinderWindowPrivate::keywordsEmpty);
  win.setSearchKeywords(QtFinder::Cmd::kQuickfix, "keywords");
  win.clearSearchKeywords();
  QCOMPARE(spy.count(), 1);
}

void QtFinderWindowPrivateTest::
    candidateIsEmpty_selectCandidate_gotSignalCandidateEmpty() {
  QtFinderWindowPrivate win;
  QSignalSpy spy(&win, &QtFinderWindowPrivate::candidateEmpty);

  win.selectCandidateAsPath(0);
  QCOMPARE(spy.count(), 1);
}

void QtFinderWindowPrivateTest::candidateisEmpty_addTwoCandidates_sizeIsTwo() {
  QtFinderWindowPrivate win;

  win.addCandidate("candidate_1");
  win.addCandidate("candidate_2");
  QCOMPARE(win.candidateSize(), 2);
}

void QtFinderWindowPrivateTest::
    hasSomeCandidates_selectCandidate_gotSelectAsPathSignal() {
  QtFinderWindowPrivate win;

  win.show();

  QSignalSpy spy(&win, &QtFinderWindowPrivate::selectedPathChanged);

  QTemporaryFile f1("f1");
  QTemporaryFile f2("f2");

  f1.open();
  f2.open();

  win.addCandidate(f1.fileName());
  win.addCandidate(f2.fileName());

  // win.selectCandidateAsPath(1);
  QTest::keyPress(&win, Qt::Key_Enter, Qt::ControlModifier);
  spy.wait(3000);

  QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(QtFinderWindowPrivateTest)
#include "QtFinderWindowPrivateTest.moc"
