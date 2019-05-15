#include <QDir>
#include <QKeyEvent>
#include <QRegExp>
#include <SearchLineEdit.h>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem::v1;

SearchLineEdit::SearchLineEdit(QWidget *parent) : QLineEdit(parent) {
  connect(this, &QLineEdit::textEdited, this,
          &SearchLineEdit::parseSearchPattern);
  connect(&delayTimer_, &QTimer::timeout, this,
          [&]() { emit searchKeyWordsChanged(keywords_, SearchRequest::kFd); });
  delayTimer_.setSingleShot(true);
}

SearchLineEdit::~SearchLineEdit() noexcept {}

void SearchLineEdit::parseSearchPattern(const QString &text) {
  /// canel search timer,and restart the timer
  /// If the user no longer enters, a delay will trigger the search.
  delayTimer_.stop();

  /// escape whitespace
  auto line = text.trimmed().replace(QRegExp("\\s+"), " ");
  QStringList list = line.split(" ");
  if (line.isEmpty() || list.empty()) {
    emit keywordsEmpty();
    return;
  }

  auto key = list.front();

  /// first, test is a absolute path or home?
  fs::path dir(key.toStdString());
  if ((fs::exists(dir) && fs::is_directory(dir) && dir.is_absolute()) ||
      (dir == "~")) {
    clear();
    emit directoryChanged(key);
    return;
  }

  if (!validateKeywords(list)) {
    return;
  }
  /// second, test is a fd request?
  if (key.toLower() == ":fd") {
    list.pop_front();
    if (list.isEmpty()) {
      return;
    }
    keywords_ = list;
    delayTimer_.start(searchDelay_);
    return;
  }
  /// now, it is a quickfix request
  return searchKeyWordsChanged(list, SearchRequest::kQuickfix);
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
  case Qt::Key_Down: {
    emit ctrlNextPressed();
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
  case Qt::Key_Up: {
    emit ctrlPrevPressed();
    return;
  }
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
bool SearchLineEdit::focusNextPrevChild(bool next) { return false; }
