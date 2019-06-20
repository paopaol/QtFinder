#include <QuickfixLabel.h>
#include <QuickfixWidget.h>

#include <Events.h>
#include <QKeyEvent>
#include <QVBoxLayout>

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

void QuickfixWidget::keyPressEvent(QKeyEvent *event) {
  auto key = key_press_event(event);
  if (key != Qt::Key_unknown) {
    emit keyPressed(key);
    return;
  }
  QListWidget::keyPressEvent(event);
}

bool QuickfixWidget::focusNextPrevChild(bool next) { return false; }
