#include <QDir>
#include <QRegExp>
#include <SearchLineEdit.h>

SearchLineEdit::SearchLineEdit(QWidget *parent) : QLineEdit(parent) {
  connect(this, &QLineEdit::textEdited, this,
          &SearchLineEdit::parseSearchPattern);
}

SearchLineEdit::~SearchLineEdit() {}

void SearchLineEdit::parseSearchPattern(const QString &text) {
  /// escape whitespace
  auto line = text.trimmed();
  if (line.isEmpty()) {
    emit searchKeyWordsChanged(QStringList());
    return;
  }
  QStringList list = text.split(QRegExp("\\s+"));
  if (list.empty()) {
    return;
  }

  auto key = list.front();
  QDir dir(key);
  if (dir.exists() && QDir::isAbsolutePath(key)) {
    emit directoryChanged(key);
    return;
  }

  /// must input at least 3 chars
  if (list.front().size() < 3) {
    setPlaceholderText("at least 3 chars");
    return;
  }
  emit searchKeyWordsChanged(list);
}
