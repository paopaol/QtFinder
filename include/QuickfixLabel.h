#ifndef QUICKFIX_LABEL_H
#define QUICKFIX_LABEL_H

#include <QLabel>

class QuickfixLabel : public QLabel {
public:
  //! Default constructor
  QuickfixLabel(QWidget *parent = nullptr);

  //! Copy constructor
  QuickfixLabel(const QuickfixLabel &other) = delete;

  //! Copy assignment operator
  QuickfixLabel &operator=(const QuickfixLabel &other) = delete;

  //! Destructor
  virtual ~QuickfixLabel() noexcept;

protected:
private:
};

#endif /* QUICKFIX_LABEL_H */
