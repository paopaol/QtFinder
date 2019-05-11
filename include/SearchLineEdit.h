#ifndef INPUT_BOX_H
#define INPUT_BOX_H

#include <QLineEdit>

class SearchLineEdit : public QLineEdit {
  Q_OBJECT
public:
  enum class KeywordsType { kQuickfix, kFd, kRg };
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

signals:
  /**
   * @brief emited when search key words changed
   * @details the input key word length must >= 3 chars
   * @return void
   */
  void searchKeyWordsChanged(const QStringList &keywords, KeywordsType type);
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
};

#endif /* INPUT_BOX_H */
