#include "QtFinderToolImpl.h"

QtFinderToolImpl::QtFinderToolImpl(QObject *parent)
    : AbstractQtFinderTool(parent), tool_(new QtFinderTool(this)) {
  connect(tool_.data(), &QtFinderTool::candidatesReady, this,
          &AbstractQtFinderTool::candidatesReady);
  connect(tool_.data(), &QtFinderTool::started, this,
          &AbstractQtFinderTool::started);
  connect(tool_.data(), &QtFinderTool::stopped, this,
          &AbstractQtFinderTool::stopped);
}

void QtFinderToolImpl::startSearchOnDirectory(const QString &absolutePath,
                                              const QStringList &keywords) {
  tool_->startSearchOnDirectory(absolutePath, keywords);
}

void QtFinderToolImpl::stop() { tool_->stop(); }
