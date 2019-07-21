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
};

void QtFinderToolTest::createSomeTempFiles_inputSomeKeywords_Works() {
  QtFinderTool tool;

  QSignalSpy spy(&tool, &QtFinderTool::candidateReady);

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

QTEST_MAIN(QtFinderToolTest)
#include "QtFinderToolTest.moc"
