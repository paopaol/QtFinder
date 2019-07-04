#include <QFileIconProvider>
#include <QuickfixLabel.h>
#include <QuickfixWidget.h>

#include <Events.h>
#include <QKeyEvent>
#include <QVBoxLayout>

namespace QtFinder {
static QListWidgetItem *createFileItem(const QString &path, const QString &text,
                                       QListWidget *parent);

QuickfixWidget::QuickfixWidget(QWidget *parent) : QListWidget(parent) {}

QuickfixWidget::~QuickfixWidget() noexcept {}

void QuickfixWidget::clear() {
  QListWidget::clear();
  currentRow_ = -1;
}

void QuickfixWidget::updateCurrentRow(SelectOpt opt) {
  switch (opt) {
  case SelectOpt::kKeep: {
    break;
  }
  case SelectOpt::kDown: {
    ++currentRow_;
    break;
  }
  case SelectOpt::kUp: {
    --currentRow_;
    break;
  }
  }
  int cnt = count();

  currentRow_ = std::min(cnt - 1, currentRow_);
  currentRow_ = std::max(0, currentRow_);
  if (currentRow_ < cnt && cnt > 0) {
    setCurrentRow(currentRow_);
  }
}

void QuickfixWidget::addCandidate(const QString &candidate) {
  addItem(candidate);
  updateCurrentRow(SelectOpt::kKeep);
}

void QuickfixWidget::keyPressEvent(QKeyEvent *event) {
  auto key = key_press_event(event);
  switch (key) {
  case Qt::Key_Down: {
    updateCurrentRow(SelectOpt::kDown);
    break;
  }
  case Qt::Key_Up: {
    updateCurrentRow(SelectOpt::kUp);
    break;
  }
  default:
    break;
  }
  if (key != Qt::Key_unknown) {
    emit shortcutKeyPressed(key);
    return;
  }
  QListWidget::keyPressEvent(event);
}

bool QuickfixWidget::focusNextPrevChild(bool next) { return false; }
} // namespace QtFinder
