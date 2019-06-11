#include <QuickfixLabel.h>
#include <QuickfixWidget.h>

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
    if (currentRow_ == -1) {
      ++currentRow_;
    }
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
  switch (event->key()) {
  case Qt::Key_Enter:
  case Qt::Key_Return:
  case Qt::Key_Tab: {
    emit keyPressed(Qt::Key_Enter);
    return;
  }
  case Qt::Key_Down: {
    emit keyPressed(Qt::Key_Down);
    return;
  }
  case Qt::Key_N:
  case Qt::Key_J: {
    if (event->modifiers() == Qt::ControlModifier) {
      emit keyPressed(Qt::Key_Down);
      return;
    }
    break;
  }
  case Qt::Key_Up: {
    emit keyPressed(Qt::Key_Up);
    return;
  }
  case Qt::Key_P:
  case Qt::Key_K: {
    if (event->modifiers() == Qt::ControlModifier) {
      emit keyPressed(Qt::Key_Up);
      return;
    }
    break;
  }
  default:
    break;
  }
  QListWidget::keyPressEvent(event);
}

bool QuickfixWidget::focusNextPrevChild(bool next) { return false; }
