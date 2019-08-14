#include <QSignalSpy>
#include <QTest>

#include <QtFinderWindow.h>
#include <private/QtFinderWindow_p.h>

class QtFinderWindowTest;
class QtFinderWindowHook : public QtFinderWindow {
  Q_OBJECT
public:
  QtFinderWindowHook(QWidget *parent = nullptr) : QtFinderWindow(parent) {}

private:
  Ui::Widget &ui() {
    Q_D(QtFinderWindow);
    return d->ui;
  }

  friend class QtFinderWindowTest;
};

class QtFinderWindowTest : public QObject {
  Q_OBJECT
private slots:
  void defaultDirectoryIsHome_setDirectory_currentDirectoryWorks();
};

void QtFinderWindowTest::
    defaultDirectoryIsHome_setDirectory_currentDirectoryWorks() {
  QtFinderWindowHook hook;
  Ui::Widget &ui = hook.d_ptr->ui;
  QSignalSpy spy();

  hook.show();

  QCOMPARE(hook.defaultDirectory(), "~");
  hook.setDefaultDirectory("c:/");
  QCOMPARE(hook.defaultDirectory(), "c:/");
}

QTEST_MAIN(QtFinderWindowTest)
#include "gui_main_window_test.moc"
