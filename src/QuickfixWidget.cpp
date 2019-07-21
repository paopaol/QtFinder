#include <QFileIconProvider>
#include <QuickfixWidget.h>

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

void QuickfixWidget::focusNextCandidate() {
  updateCurrentRow(SelectOpt::kDown);
}

void QuickfixWidget::focusPreviousCandidate() {
  updateCurrentRow(SelectOpt::kUp);
}


bool QuickfixWidget::focusNextPrevChild(bool next) { return false; }
} // namespace QtFinder
