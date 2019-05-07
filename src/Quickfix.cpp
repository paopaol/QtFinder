#include <Quickfix.h>
#include <QuickfixLabel.h>

#include <QVBoxLayout>

Quickfix::Quickfix(const QWidget *parent) {}

Quickfix::~Quickfix() noexcept {}

void Quickfix::appendLines(const QStringList &lineList) {
  lineList_.append(lineList);
}

void Quickfix::show() {
  QVBoxLayout *layout = new QVBoxLayout(this);

  for (auto &line : lineList_) {
    QuickfixLabel *label = new QuickfixLabel(this);
    label->setText(line);
  }
  setLayout(layout);
  show();
}
