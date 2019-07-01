#include <QSignalSpy>
#include <QTest>

#include <Events.h>
#include <SearchLineEdit.h>
#include <private/QtFinderWindow_p.h>

class GuiMainWindowPrivateTest : public QObject {
  Q_OBJECT

private slots:
  void keywordsIsEmpty_inputKeywords_gotSignalKeywordsChanged();
  void keywordsIsEmpty_setKeywords_gotSignalKeywordsChanged();
  void keywordsNotEmpty_clearKeyWords_gotSignalKeywordsEmpty();
  void typeShortcutKey_gotSignalShortcutKeyPressed();
  void candidatesIsEmpty_insert2Candidates_display2Candidates();
  void quickfixCurrentRowStayHere_moveDownUp_Works();

private:
  template <class T> void testSimulateShortcutKey(T *widget);
};

void GuiMainWindowPrivateTest::
    keywordsIsEmpty_inputKeywords_gotSignalKeywordsChanged() {
  SearchLineEdit lineEdit;

  lineEdit.show();

  QSignalSpy spy(&lineEdit, &SearchLineEdit::searchKeyWordsChanged);

  lineEdit.setFdCmdTriggerDelay(1000);

  QTest::keyClicks(&lineEdit, ":fd search keywords");

  spy.wait(2000);

  QCOMPARE(spy.count(), 1);

  QList<QVariant> arguments = spy.takeFirst();
  QCOMPARE(arguments.size(), 2);

  auto keywords = arguments.at(0).toStringList();
  auto cmd = qvariant_cast<QtFinder::Cmd>(arguments.at(1));
  QCOMPARE(cmd, QtFinder::Cmd::kFd);
  QCOMPARE(keywords, QStringList() << "search"
                                   << "keywords");
}

void GuiMainWindowPrivateTest::
    keywordsIsEmpty_setKeywords_gotSignalKeywordsChanged() {
  SearchLineEdit lineEdit;

  lineEdit.show();

  QSignalSpy spy(&lineEdit, &SearchLineEdit::searchKeyWordsChanged);

  lineEdit.setFdCmdTriggerDelay(1000);

  lineEdit.setText("keywords");

  spy.wait(2000);

  QCOMPARE(spy.count(), 1);

  QList<QVariant> arguments = spy.takeFirst();
  QCOMPARE(arguments.size(), 2);

  auto keywords = arguments.at(0).toStringList();
  auto cmd = qvariant_cast<QtFinder::Cmd>(arguments.at(1));
  QCOMPARE(cmd, QtFinder::Cmd::kQuickfix);
  QCOMPARE(keywords, QStringList() << "keywords");
}

void GuiMainWindowPrivateTest::
    keywordsNotEmpty_clearKeyWords_gotSignalKeywordsEmpty() {

  SearchLineEdit lineEdit;

  lineEdit.show();

  lineEdit.setFdCmdTriggerDelay(1000);

  QSignalSpy spy(&lineEdit, &SearchLineEdit::keywordsEmpty);

  QTest::keyClicks(&lineEdit, ":fd search keywords", Qt::NoModifier, 100);
  QTest::keyPress(&lineEdit, 'a', Qt::ControlModifier, 200);
  QTest::keyPress(&lineEdit, Qt::Key_Delete, Qt::NoModifier, 3000);
  QCOMPARE(spy.count(), 1);
}

void GuiMainWindowPrivateTest::typeShortcutKey_gotSignalShortcutKeyPressed() {
  SearchLineEdit lineEdit;
  QuickfixWidget quickfixWidget;

  testSimulateShortcutKey(&lineEdit);
  testSimulateShortcutKey(&quickfixWidget);
}

template <class T>
void GuiMainWindowPrivateTest::testSimulateShortcutKey(T *widget) {
  widget->show();
  QSignalSpy spy(widget, &T::shortcutKeyPressed);

  for (auto &shortcutKey : shortcutKeyTable()) {
    QTest::keyPress(widget, shortcutKey.Key, shortcutKey.Modifier, 10);
    QCOMPARE(spy.count(), 1);

    auto arguments = spy.takeFirst();

    QCOMPARE(qvariant_cast<Qt::Key>(arguments.at(0)), shortcutKey.AliasKey);
  }
}

void GuiMainWindowPrivateTest::
    candidatesIsEmpty_insert2Candidates_display2Candidates() {
  QuickfixWidget quickfixWidget;

  quickfixWidget.show();

  QCOMPARE(quickfixWidget.count(), 0);
  quickfixWidget.addCandidate("d:/123/111.txt");
  quickfixWidget.addCandidate("c:/123/111/txt");

  QCOMPARE(quickfixWidget.count(), 2);
}

void GuiMainWindowPrivateTest::quickfixCurrentRowStayHere_moveDownUp_Works() {
  QuickfixWidget quickfixWidget;
  quickfixWidget.show();
  quickfixWidget.addCandidate("d:/123/111.txt");
  quickfixWidget.addCandidate("c:/123/111/txt");

  QCOMPARE(quickfixWidget.currentRow(), 0);

  QTest::keyPress(&quickfixWidget, Qt::Key_Up, Qt::NoModifier, 500);
  QCOMPARE(quickfixWidget.currentRow(), 0);

  QTest::keyPress(&quickfixWidget, Qt::Key_Down, Qt::NoModifier, 500);
  QCOMPARE(quickfixWidget.currentRow(), 1);

  QTest::keyPress(&quickfixWidget, Qt::Key_Down, Qt::NoModifier, 500);
  QCOMPARE(quickfixWidget.currentRow(), 1);
}

QTEST_MAIN(GuiMainWindowPrivateTest)
#include "ui_main_delegete_test.moc"
