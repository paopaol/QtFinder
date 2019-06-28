#include <QSignalSpy>
#include <QTest>

#include <private/QtFinderWindow_p.h>

class TestGuiMainWindowPrivate;
class QtFinderWindowPrivateHook : public QtFinderWindowPrivate {
public:
  QtFinderWindowPrivateHook() : dummy(new QWidget), QtFinderWindowPrivate() {}
  friend class TestGuiMainWindowPrivate;

private:
  QWidget *dummy{nullptr};
};

class TestGuiMainWindowPrivate : public QObject {
  Q_OBJECT

private slots:
  void keywordsIsEmpty_inputKeywords_gotSinalAfterSomeDelay();
  void keywordsNotEmpty_clearKeyWords_gotEmptySignal();
};

void TestGuiMainWindowPrivate::
    keywordsIsEmpty_inputKeywords_gotSinalAfterSomeDelay() {

  QtFinderWindowPrivateHook hook;

  Ui::Widget &ui = hook.ui;

  ui.setupUi(hook.dummy);

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

void TestGuiMainWindowPrivate::keywordsNotEmpty_clearKeyWords_gotEmptySignal() {

  QtFinderWindowPrivateHook hook;

  Ui::Widget &ui = hook.ui;

  ui.setupUi(hook.dummy);
  ui.searchLineEdit->setKeywordsChangedMaxDelay(1000);

  QSignalSpy spy(ui.searchLineEdit, &SearchLineEdit::keywordsEmpty);

  QTest::keyClicks(ui.searchLineEdit, ":fd search keywords");
  QTest::keyPress(ui.searchLineEdit, 'a', Qt::ControlModifier, 200);
  QTest::keyPress(ui.searchLineEdit, Qt::Key_Delete );
  QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(TestGuiMainWindowPrivate)
#include "ui_main_delegete_test.moc"
