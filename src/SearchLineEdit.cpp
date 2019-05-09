#include <QRegExp>
#include <SearchLineEdit.h>

SearchLineEdit::SearchLineEdit(QWidget *parent) : QLineEdit(parent) {
  connect(this, &QLineEdit::textChanged, this,
          &SearchLineEdit::parseSearchPattern);
}

SearchLineEdit::~SearchLineEdit() {}

void SearchLineEdit::parseSearchPattern(const QString &text) {
  QStringList list = text.trimmed().split(QRegExp("\\s+"));
  /// escape whitespace
  if (list.empty()) {
    return;
  }
  /// must input at least 3 chars
  if (list.front().size() < 3) {
    setPlaceholderText("at least 3 chars");
    return;
  }

  emit searchKeyWordsChanged(list);
}
