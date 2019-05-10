#ifndef QUICKFIX_H
#define QUICKFIX_H

#include <QListWidget>

class QuickfixLabel;
/**@brief quickfix is like vim quickfix
 */
class QuickfixWidget : public QListWidget {
public:
  enum class SelectOpt { kUp, kDown, kKeep };

  QuickfixWidget(const QWidget *parent = nullptr);
  QuickfixWidget(const QuickfixWidget &other) = delete;
  QuickfixWidget &operator=(const QuickfixWidget &other) = delete;
  virtual ~QuickfixWidget() noexcept;

  void clear();
  void updateCurrentRow(SelectOpt opt);

private:
  int currentRow_{-1};
};

#endif /* QUICKFIX_H */
