#ifndef __QTFINDERTOOL_H_
#define __QTFINDERTOOL_H_

#include <QObject>
#include <QString>

class QProcess;
class QtFinderTool final : public QObject {
  Q_OBJECT
public:
  QtFinderTool(QObject *parent = nullptr);
  virtual ~QtFinderTool();

  void startSearchOnDirectory(const QString &absolutePath,
                              const QStringList &keywords);
  void stop();

signals:
  void candidateReady(const QString &candidate);

private:
  QScopedPointer<QProcess> fd_;
};

#endif // __QTFINDERTOOL_H_
