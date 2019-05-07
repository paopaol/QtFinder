#include <QDebug>
#include <QtFinderWindow.h>

QtFinderWindow::QtFinderWindow(QWidget *parent) : QWidget(parent) {
  uiWidget.setupUi(this);
  connect(uiWidget.searchLineEdit, &SearchLineEdit::searchKeyWordsChanged, this,
          &QtFinderWindow::onSearchKeyWordsChanged);
}
QtFinderWindow::~QtFinderWindow() {}

void QtFinderWindow::onSearchKeyWordsChanged(const QStringList &keyWords) {
  qDebug() << keyWords;
  QString pattern;
  QTextStream st(&pattern);
  for (auto &key : keyWords) {
    st << key << ".*";
  }
  QString cmd = QString("fd -p '%1'").arg(pattern);
  system(cmd.toStdString().c_str());
}
