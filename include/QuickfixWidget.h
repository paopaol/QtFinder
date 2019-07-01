#ifndef QUICKFIX_H
#define QUICKFIX_H

#include <QListWidget>

class QuickfixLabel;
/**@brief quickfix is like vim quickfix
 */
class QuickfixWidget : public QListWidget {
  Q_OBJECT
public:
  enum class SelectOpt { kUp, kDown, kKeep };

  QuickfixWidget(QWidget *parent = nullptr);
  QuickfixWidget(const QuickfixWidget &other) = delete;
  QuickfixWidget &operator=(const QuickfixWidget &other) = delete;
  virtual ~QuickfixWidget() noexcept;

  void clear();
  void updateCurrentRow(SelectOpt opt);
  void addCandidate(const QString &candidate);

signals:
  /**@brief emited when some key pressed
     ctrl+n/j will convert to down key
     ctrl+p/k will convert to up key
     enter/return/tab will convert to enter
  */
  void shortcutKeyPressed(Qt::Key key);

protected:
  void keyPressEvent(QKeyEvent *) override;
  bool focusNextPrevChild(bool next);

private:
  int currentRow_{-1};
};

#endif /* QUICKFIX_H */
