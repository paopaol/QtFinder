#ifndef INPUT_BOX_H
#define INPUT_BOX_H

#include <QLineEdit>

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

  /**
   * @brief emited when search key words changed
   * @details the input key word length must >= 3 chars
   * @return void
   */
signals:
  void searchKeyWordsChanged(const QStringList &keywords);

private:
  void parseSearchPattern(const QString &text);
};

#endif /* INPUT_BOX_H */
