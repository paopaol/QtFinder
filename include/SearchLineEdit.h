#ifndef INPUT_BOX_H
#define INPUT_BOX_H

#include "QtFinderCmd.h"
#include <QLineEdit>
#include <QMap>
#include <QTimer>

template <class T> inline QList<T> &cdrList(QList<T> &list) {
  if (list.size() > 0) {
    list.pop_front();
  }
  return list;
}
namespace QtFinder {
class SearchLineEdit : public QLineEdit {
  Q_OBJECT
public:
  //! Default constructor
  SearchLineEdit(QWidget *parent = nullptr);
  //! Copy constructor
  SearchLineEdit(const SearchLineEdit &other) = delete;
  //! Copy assignment operator
  SearchLineEdit &operator=(const SearchLineEdit &other) = delete;
  //! Destructor
  virtual ~SearchLineEdit() noexcept;

  void setFdCmdTriggerDelay(int delayMs);

protected:
  void keyPressEvent(QKeyEvent *) override;
  bool focusNextPrevChild(bool next) override;

signals:
  /**
   * @brief emited when search key words changed
   * @details the input key word length must >= 3 chars
   * @return void
   */
  void searchKeywordsChanged(const QStringList &keywords, QtFinder::Cmd cmd);
  /**@brief search line edit buffer is empty
   */
  void keywordsEmpty();

  /**@brief emited when some key pressed
     ctrl+n/j will convert to down key
     ctrl+p/k will convert to up key
     enter/return/tab will convert to enter
   */
  void shortcutKeyPressed(Qt::Key key);

private:
  typedef std::function<void(const QStringList &keywords)> cmdEmiter;
  void parseUserInput(const QString &text);
  void emitQtFinderCmd(QStringList &keywords);

  /**@brief if keywords is validate successed, then,it will
     emit searchKeywordsChanged signal with a @see QtFinder::Cmd
   */
  void fdCmdEmit(const QStringList &keywords);
  void directoryCmdEmit(const QStringList &keywords);
  void quickfixCmdEmit(const QStringList &keywords);

  QStringList keywords_;
  /**@brief Delay in triggering search behavior after entering a keyword
   */
  int searchDelay_{600};
  QTimer delayTimer_;
  QMap<QString, cmdEmiter> cmdTable_;
};
} // namespace QtFinder

#endif /* INPUT_BOX_H */
