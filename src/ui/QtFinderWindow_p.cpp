#include <private/QtFinderWindow_p.h>

QtFinderWindowPrivate::QtFinderWindowPrivate(QWidget *parent)
    : QWidget(parent) {
  ui.setupUi(this);
}

QtFinderWindowPrivate::~QtFinderWindowPrivate() noexcept {}
