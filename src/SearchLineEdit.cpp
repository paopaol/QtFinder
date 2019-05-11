#include <QDir>
#include <QKeyEvent>
#include <QRegExp>
#include <SearchLineEdit.h>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem::v1;

SearchLineEdit::SearchLineEdit(QWidget *parent) : QLineEdit(parent) {
  connect(this, &QLineEdit::textEdited, this,
          &SearchLineEdit::parseSearchPattern);
}

SearchLineEdit::~SearchLineEdit() {}

void SearchLineEdit::parseSearchPattern(const QString &text) {
  /// escape whitespace
  auto line = text.trimmed();
  if (line.isEmpty()) {
    emit keywordsEmpty();
    return;
  }
  QStringList list = text.split(QRegExp("\\s+"));
  if (list.empty()) {
    emit keywordsEmpty();
    return;
  }

  auto key = list.front();
  list.pop_front();

  /// first, test is a absolute path?
  fs::path dir(key.toStdString());
  if (fs::exists(dir) && fs::is_directory(dir) && dir.is_absolute() || dir == "~") {
    clear();
    emit directoryChanged(key);
    return;
  }

  if (!validateKeywords(list)) {
    return;
  }
  /// second, test is a fd request?
  if (key == ":fd") {
    emit searchKeyWordsChanged(list, KeywordsType::kFd);
    return;
  }
  /// third, test is a rg request?
  if (key == ":rg") {
    emit searchKeyWordsChanged(list, KeywordsType::kQuickfix);
    return;
  }
  /// now, it is a quickfix request
  return searchKeyWordsChanged(list, KeywordsType::kQuickfix);
}

bool SearchLineEdit::validateKeywords(const QStringList &keywords) {
  /// must input at least 3 chars
  if (keywords.isEmpty() || keywords.front().size() < 3) {
    // setPlaceholderText("at least 3 chars");
    return false;
  }
  return true;
}

void SearchLineEdit::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
  case Qt::Key_Tab: {
    emit tabKeyPressed();
    return;
  }
  case Qt::Key_Down:
  case Qt::Key_N:
  case Qt::Key_J: {
    if (event->modifiers() == Qt::ControlModifier) {
      emit ctrlNextPressed();
      return;
    }
    break;
  }
  case Qt::Key_Up:
  case Qt::Key_P:
  case Qt::Key_K: {
    if (event->modifiers() == Qt::ControlModifier) {
      emit ctrlPrevPressed();
      return;
    }
    break;
  }
  default:
    break;
  }
  QLineEdit::keyPressEvent(event);
}
