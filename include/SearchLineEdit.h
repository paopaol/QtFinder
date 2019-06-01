#ifndef INPUT_BOX_H
#define INPUT_BOX_H

#include "QtFinderCmd.h"
#include <QLineEdit>
#include <QTimer>

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

protected:
  void keyPressEvent(QKeyEvent *) override;
  bool focusNextPrevChild(bool next) override;

signals:
  /**
   * @brief emited when search key words changed
   * @details the input key word length must >= 3 chars
   * @return void
   */
  void searchKeyWordsChanged(const QStringList &keywords, QtFinder::Cmd cmd);
  /**@brief search line edit buffer is empty
   */
  void keywordsEmpty();
  /**
   * @name directoryChanged find in directory changed
   * @param directory  directory
   * @return void
   */
  void directoryChanged(const QString &directory);
  void tabKeyPressed();
  /**@brief ctrl-n/j/PgUp
   */
  void ctrlNextPressed();
  /**@brief ctrl-p/k/PgDn
   */
  void ctrlPrevPressed();

private:
  void parseSearchPattern(const QString &text);
  bool validateKeywords(const QStringList &keywords);

  QStringList keywords_;
  /**@brief Delay in triggering search behavior after entering a keyword
   */
  int searchDelay_{600};
  QTimer delayTimer_;
};

#endif /* INPUT_BOX_H */
