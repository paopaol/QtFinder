#include <QSignalSpy>
#include <QTest>
#include <private/QtFinderWindow_p.h>

class QtFinderWindowPrivateTest : public QObject {
  Q_OBJECT
private slots:
  void defaultDirectoryIsHome();
  void keywordsIsEmpty_setFdKeywords_gotSignalFdKeywordsChanged();
  void keywordsNotEmpty_clearKeywords_gotSignalKeywordsEmpty();
  void candidateIsEmpty_openCandidate_gotSignalCandidateEmpty();
};

void QtFinderWindowPrivateTest::defaultDirectoryIsHome() {
  QtFinderWindowPrivate win;
  QCOMPARE(win.currentDirectory(), QString("~"));
}

void QtFinderWindowPrivateTest::
    keywordsIsEmpty_setFdKeywords_gotSignalFdKeywordsChanged() {
  QtFinderWindowPrivate win;

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
    candidateIsEmpty_openCandidate_gotSignalCandidateEmpty() {
  QtFinderWindowPrivate win;
  QSignalSpy spy(&win, &QtFinderWindowPrivate::candidateEmpty);

  win.openCandidateAsPath(0);
  QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(QtFinderWindowPrivateTest)
#include "QtFinderWindowPrivateTest.moc"
