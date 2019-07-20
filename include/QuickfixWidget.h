#ifndef QUICKFIX_H
#define QUICKFIX_H

#include <QListWidget>

class QuickfixLabel;

namespace QtFinder {
/**@brief quickfix is like vim quickfix
 */
class QuickfixWidget : public QListWidget {
  Q_OBJECT
public:
  QuickfixWidget(QWidget *parent = nullptr);
  QuickfixWidget(const QuickfixWidget &other) = delete;
  QuickfixWidget &operator=(const QuickfixWidget &other) = delete;
  virtual ~QuickfixWidget() noexcept;

  void clear();
  void addCandidate(const QString &candidate);
  void focusNextCandidate();
  void focusPreviousCandidate();

protected:
  bool focusNextPrevChild(bool next);

private:
  enum class SelectOpt { kUp, kDown, kKeep };

  void updateCurrentRow(SelectOpt opt);

  int currentRow_{-1};
};
} // namespace QtFinder

#endif /* QUICKFIX_H */
