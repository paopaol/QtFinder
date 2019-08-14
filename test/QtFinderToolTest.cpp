#include <QDir>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <QTest>
#include <QtFinderTool.h>

class QtFinderToolTest : public QObject {
  Q_OBJECT
private slots:
  void createSomeTempFiles_inputSomeKeywords_Works();
  void startSearch_goStartedSignal();
  void stopSearch_goStoppedSignal();
};

void QtFinderToolTest::createSomeTempFiles_inputSomeKeywords_Works() {
  QtFinderTool tool;

  QSignalSpy spy(&tool, &QtFinderTool::candidatesReady);

  QTemporaryDir tempDir;
  QTemporaryFile tempFile;
  tempFile.setFileTemplate(tempDir.filePath("QtFinder-test"));
  tempFile.open();

  QDir dir(tempFile.fileName());
  dir.cdUp();
  QString path = dir.absolutePath();

  tool.startSearchOnDirectory(path, QStringList() << "QtFinder-test");

  spy.wait(1000);

  QCOMPARE(spy.count(), 1);
}

void QtFinderToolTest::startSearch_goStartedSignal() {
  QtFinderTool tool;

  QSignalSpy spy(&tool, &QtFinderTool::started);

  tool.startSearchOnDirectory("absolutePath", QStringList() << "key");
  QCOMPARE(spy.count(), 1);
}

void QtFinderToolTest::stopSearch_goStoppedSignal() {
  QtFinderTool tool;

  QSignalSpy spy(&tool, &QtFinderTool::stopped);

  tool.startSearchOnDirectory("absolutePath", QStringList() << "key");
  tool.stop();
  QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(QtFinderToolTest)
#include "QtFinderToolTest.moc"
