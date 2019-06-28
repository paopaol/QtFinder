#include <QSignalSpy>
#include <QTest>

#include <Events.h>
#include <private/QtFinderWindow_p.h>

class TestGuiMainWindowPrivate;
class QtFinderWindowPrivateHook : public QtFinderWindowPrivate {
public:
  QtFinderWindowPrivateHook(QWidget *parent = nullptr)
      : QtFinderWindowPrivate(parent) {}
  friend class TestGuiMainWindowPrivate;
};

class TestGuiMainWindowPrivate : public QObject {
  Q_OBJECT

private slots:
  void keywordsIsEmpty_inputKeywords_gotSignalKeywordsChanged();
  void keywordsNotEmpty_clearKeyWords_gotSignalKeywordsEmpty();
  void typeShortcutKey_gotSignalShortcutKeyPressed();

private:
  template <class T> void testSimulateShortcutKey(T *widget);
};

void TestGuiMainWindowPrivate::
    keywordsIsEmpty_inputKeywords_gotSignalKeywordsChanged() {

  QtFinderWindowPrivateHook hook;

  Ui::Widget &ui = hook.ui;

  QSignalSpy spy(ui.searchLineEdit, &SearchLineEdit::searchKeyWordsChanged);

  ui.searchLineEdit->setKeywordsChangedMaxDelay(1000);

  QTest::keyClicks(ui.searchLineEdit, ":fd search keywords");

  spy.wait(2000);

  QList<QVariant> arguments = spy.takeFirst();
  QCOMPARE(arguments.size(), 2);

  auto cmd = qvariant_cast<QtFinder::Cmd>(arguments.at(1));
  QCOMPARE(cmd, QtFinder::Cmd::kQuickfix);

  arguments = spy.takeFirst();
  cmd = qvariant_cast<QtFinder::Cmd>(arguments.at(1));
  QCOMPARE(cmd, QtFinder::Cmd::kQuickfix);

  arguments = spy.takeFirst();
  auto keywords = arguments.at(0).toStringList();
  cmd = qvariant_cast<QtFinder::Cmd>(arguments.at(1));
  QCOMPARE(cmd, QtFinder::Cmd::kFd);
  QCOMPARE(keywords, QStringList() << "search"
                                   << "keywords");
}

void TestGuiMainWindowPrivate::
    keywordsNotEmpty_clearKeyWords_gotSignalKeywordsEmpty() {

  QtFinderWindowPrivateHook hook;

  Ui::Widget &ui = hook.ui;

  hook.show();

  ui.searchLineEdit->setKeywordsChangedMaxDelay(1000);

  QSignalSpy spy(ui.searchLineEdit, &SearchLineEdit::keywordsEmpty);

  QTest::keyClicks(ui.searchLineEdit, ":fd search keywords", Qt::NoModifier,
                   100);
  QTest::keyPress(ui.searchLineEdit, 'a', Qt::ControlModifier, 200);
  QTest::keyPress(ui.searchLineEdit, Qt::Key_Delete, Qt::NoModifier, 3000);
  QCOMPARE(spy.count(), 1);
}

void TestGuiMainWindowPrivate::typeShortcutKey_gotSignalShortcutKeyPressed() {
  QtFinderWindowPrivateHook hook;

  Ui::Widget &ui = hook.ui;

  hook.show();

  testSimulateShortcutKey(ui.searchLineEdit);
  testSimulateShortcutKey(ui.quickfixWidget);
}

template <class T>
void TestGuiMainWindowPrivate::testSimulateShortcutKey(T *widget) {
  QSignalSpy spy(widget, &T::shortcutKeyPressed);

  for (auto &shortcutKey : shortcutKeyTable()) {
    QTest::keyPress(widget, shortcutKey.Key, shortcutKey.Modifier, 10);
    QCOMPARE(spy.count(), 1);

    auto arguments = spy.takeFirst();

    QCOMPARE(qvariant_cast<Qt::Key>(arguments.at(0)), shortcutKey.AliasKey);
  }
}

QTEST_MAIN(TestGuiMainWindowPrivate)
#include "ui_main_delegete_test.moc"
