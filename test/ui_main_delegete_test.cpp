#include <QSignalSpy>
#include <QTest>

#include <Events.h>
#include <private/QtFinderWindow_p.h>

class GuiMainWindowPrivateTest;
class QtFinderWindowPrivateHook : public QtFinderWindowPrivate {
public:
  QtFinderWindowPrivateHook(QWidget *parent = nullptr)
      : QtFinderWindowPrivate(parent) {}
  friend class GuiMainWindowPrivateTest;
};

class GuiMainWindowPrivateTest : public QObject {
  Q_OBJECT

private slots:
  void keywordsIsEmpty_inputKeywords_gotSignalKeywordsChanged();
  void keywordsNotEmpty_clearKeyWords_gotSignalKeywordsEmpty();
  void typeShortcutKey_gotSignalShortcutKeyPressed();
  void candidatesIsEmpty_insert2Candidates_display2Candidates();
  void quickfixCurrentRowStayHere_moveDownUp_Works();

private:
  template <class T> void testSimulateShortcutKey(T *widget);
};

void GuiMainWindowPrivateTest::
    keywordsIsEmpty_inputKeywords_gotSignalKeywordsChanged() {

  QtFinderWindowPrivateHook hook;

  Ui::Widget &ui = hook.ui;

  QSignalSpy spy(ui.searchLineEdit, &SearchLineEdit::searchKeyWordsChanged);

  ui.searchLineEdit->setFdCmdTriggerDelay(1000);

  QTest::keyClicks(ui.searchLineEdit, ":fd search keywords");

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
    keywordsNotEmpty_clearKeyWords_gotSignalKeywordsEmpty() {

  QtFinderWindowPrivateHook hook;

  Ui::Widget &ui = hook.ui;

  hook.show();

  ui.searchLineEdit->setFdCmdTriggerDelay(1000);

  QSignalSpy spy(ui.searchLineEdit, &SearchLineEdit::keywordsEmpty);

  QTest::keyClicks(ui.searchLineEdit, ":fd search keywords", Qt::NoModifier,
                   100);
  QTest::keyPress(ui.searchLineEdit, 'a', Qt::ControlModifier, 200);
  QTest::keyPress(ui.searchLineEdit, Qt::Key_Delete, Qt::NoModifier, 3000);
  QCOMPARE(spy.count(), 1);
}

void GuiMainWindowPrivateTest::typeShortcutKey_gotSignalShortcutKeyPressed() {
  QtFinderWindowPrivateHook hook;

  Ui::Widget &ui = hook.ui;

  hook.show();

  testSimulateShortcutKey(ui.searchLineEdit);
  testSimulateShortcutKey(ui.quickfixWidget);
}

template <class T>
void GuiMainWindowPrivateTest::testSimulateShortcutKey(T *widget) {
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
  QtFinderWindowPrivateHook hook;
  Ui::Widget &ui = hook.ui;

  hook.show();

  QCOMPARE(ui.quickfixWidget->count(), 0);
  ui.quickfixWidget->addCandidate("d:/123/111.txt");
  ui.quickfixWidget->addCandidate("c:/123/111/txt");

  QCOMPARE(ui.quickfixWidget->count(), 2);
}

void GuiMainWindowPrivateTest::quickfixCurrentRowStayHere_moveDownUp_Works() {
  QtFinderWindowPrivateHook hook;
  Ui::Widget &ui = hook.ui;

  hook.show();
  ui.quickfixWidget->addCandidate("d:/123/111.txt");
  ui.quickfixWidget->addCandidate("c:/123/111/txt");

  QCOMPARE(ui.quickfixWidget->currentRow(), 0);

  QTest::keyPress(ui.quickfixWidget, Qt::Key_Up, Qt::NoModifier, 500);
  QCOMPARE(ui.quickfixWidget->currentRow(), 0);

  QTest::keyPress(ui.quickfixWidget, Qt::Key_Down, Qt::NoModifier, 500);
  QCOMPARE(ui.quickfixWidget->currentRow(), 1);

  QTest::keyPress(ui.quickfixWidget, Qt::Key_Down, Qt::NoModifier, 500);
  QCOMPARE(ui.quickfixWidget->currentRow(), 1);
}

QTEST_MAIN(GuiMainWindowPrivateTest)
#include "ui_main_delegete_test.moc"
