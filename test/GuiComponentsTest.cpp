#include <QSignalSpy>
#include <QTest>

#include <Events.h>
#include <QtFinderWindow.h>
#include <SearchLineEdit.h>

class GuiComponentsTest : public QObject {
  Q_OBJECT

private slots:
  void keywordsIsEmpty_inputKeywords_gotSignalKeywordsChanged();
  void keywordsIsEmpty_setKeywords_gotSignalKeywordsChanged();
  void keywordsNotEmpty_clearKeyWords_gotSignalKeywordsEmpty();
  void candidatesIsEmpty_insert2Candidates_display2Candidates();
};

void GuiComponentsTest::
    keywordsIsEmpty_inputKeywords_gotSignalKeywordsChanged() {
  QtFinder::SearchLineEdit lineEdit;

  lineEdit.show();

  QSignalSpy spy(&lineEdit, &QtFinder::SearchLineEdit::searchKeywordsChanged);

  lineEdit.setFdCmdTriggerDelay(1000);

  QTest::keyClicks(&lineEdit, ":fd search keywords");

  spy.wait(2000);

  QCOMPARE(spy.count(), 1);

  QList<QVariant> arguments = spy.takeFirst();
  QCOMPARE(arguments.size(), 2);

  QtFinder::Cmd cmd = qvariant_cast<QtFinder::Cmd>(arguments.at(0));
  auto keywords = arguments.at(1).toStringList();
  QCOMPARE(cmd, QtFinder::Cmd::kFd);
  QCOMPARE(keywords, QStringList() << "search"
                                   << "keywords");
}

void GuiComponentsTest::keywordsIsEmpty_setKeywords_gotSignalKeywordsChanged() {
  QtFinder::SearchLineEdit lineEdit;

  lineEdit.show();

  QSignalSpy spy(&lineEdit, &QtFinder::SearchLineEdit::searchKeywordsChanged);

  lineEdit.setFdCmdTriggerDelay(1000);

  lineEdit.setText("keywords");

  spy.wait(2000);

  QCOMPARE(spy.count(), 1);

  QList<QVariant> arguments = spy.takeFirst();
  QCOMPARE(arguments.size(), 2);

  auto cmd = qvariant_cast<QtFinder::Cmd>(arguments.at(0));
  auto keywords = arguments.at(1).toStringList();
  QCOMPARE(cmd, QtFinder::Cmd::kQuickfix);
  QCOMPARE(keywords, QStringList() << "keywords");
}

void GuiComponentsTest::
    keywordsNotEmpty_clearKeyWords_gotSignalKeywordsEmpty() {

  QtFinder::SearchLineEdit lineEdit;

  lineEdit.show();

  lineEdit.setFdCmdTriggerDelay(1000);

  QSignalSpy spy(&lineEdit, &QtFinder::SearchLineEdit::keywordsEmpty);

  QTest::keyClicks(&lineEdit, ":fd search keywords", Qt::NoModifier, 100);
  QTest::keyPress(&lineEdit, 'a', Qt::ControlModifier, 200);
  QTest::keyPress(&lineEdit, Qt::Key_Delete, Qt::NoModifier, 3000);
  QCOMPARE(spy.count(), 1);
}

void GuiComponentsTest::
    candidatesIsEmpty_insert2Candidates_display2Candidates() {
  QtFinder::QuickfixWidget quickfixWidget;

  quickfixWidget.show();

  QCOMPARE(quickfixWidget.count(), 0);
  quickfixWidget.addCandidate("d:/123/111.txt");
  quickfixWidget.addCandidate("c:/123/111/txt");

  QCOMPARE(quickfixWidget.count(), 2);
}

QTEST_MAIN(GuiComponentsTest)
#include "GuiComponentsTest.moc"
