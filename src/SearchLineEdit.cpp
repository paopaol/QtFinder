#include <QDir>
#include <QKeyEvent>
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
    clear();
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

void SearchLineEdit::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
  case Qt::Key_Tab: {
    emit tabKeyPressed();
    return;
  }
  case Qt::Key_N:
  case Qt::Key_J: {
    if (event->modifiers() == Qt::ControlModifier) {
      emit ctrlNextPressed();
      return;
    }
    break;
  }
  case Qt::Key_P:
  case Qt::Key_K: {
    if (event->modifiers() == Qt::ControlModifier) {
      emit ctrlPrevPressed();
    }
    break;
  }
  default:
    break;
  }
  QLineEdit::keyPressEvent(event);
}
