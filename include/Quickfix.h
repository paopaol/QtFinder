#ifndef QUICKFIX_H
#define QUICKFIX_H

#include <QWidget>

class QuickfixLabel;
/**@brief quickfix is like vim quickfix
 */
class Quickfix : public QWidget {
public:
  //! Default constructor
  Quickfix(const QWidget *parent = nullptr);

  //! Copy constructor
  Quickfix(const Quickfix &other) = delete;

  //! Copy assignment operator
  Quickfix &operator=(const Quickfix &other) = delete;

  //! Destructor
  virtual ~Quickfix() noexcept;

  void appendLines(const QStringList &lineList);

  void show();

private:
  QStringList lineList_;
};

#endif /* QUICKFIX_H */
