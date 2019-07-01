#include <Events.h>
#include <QDir>
#include <QKeyEvent>
#include <QRegExp>
#include <SearchLineEdit.h>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem::v1;

static bool isAbsDirOrHome(const QString &key);

SearchLineEdit::SearchLineEdit(QWidget *parent) : QLineEdit(parent) {
  qRegisterMetaType<QtFinder::Cmd>("QtFinder::Cmd");
  qRegisterMetaType<Qt::Key>("Qt::Key");

  connect(this, &QLineEdit::textChanged, this, &SearchLineEdit::parseUserInput);
  connect(&delayTimer_, &QTimer::timeout, this,
          [&]() { emit searchKeyWordsChanged(keywords_, QtFinder::Cmd::kFd); });
  delayTimer_.setSingleShot(true);

  using namespace std::placeholders;
  QMap<QString, cmdEmiter> cmdTable = {
      {":fd", std::bind(&SearchLineEdit::fdCmdEmit, this, _1)},
      {":quickfix", std::bind(&SearchLineEdit::quickfixCmdEmit, this, _1)},
      {":directory", std::bind(&SearchLineEdit::directoryCmdEmit, this, _1)}};
  cmdTable_ = cmdTable;
}

SearchLineEdit::~SearchLineEdit() noexcept {}

void SearchLineEdit::setFdCmdTriggerDelay(int delayMs) {
  searchDelay_ = delayMs;
}

void SearchLineEdit::parseUserInput(const QString &text) {
  /// canel search timer,and restart the timer
  /// If the user no longer enters, a delay will trigger the search.
  delayTimer_.stop();

  /// escape whitespace
  auto line = text.trimmed().replace(QRegExp("\\s+"), " ");
  QStringList keywords = line.split(" ");
  if (line.isEmpty() || keywords.empty()) {
    emit keywordsEmpty();
    return;
  }
  emitQtFinderCmd(keywords);
}

void SearchLineEdit::emitQtFinderCmd(QStringList &keywords) {
  QString cmdString;
  auto key = keywords.front();
  if (isAbsDirOrHome(key)) {
    cmdString = ":directory";
    keywords.prepend(cmdString);
  } else {
    cmdString = key.toLower();
  }
  for (auto cmd = cmdTable_.begin(); cmd != cmdTable_.end(); cmd++) {
    if (cmd.key().startsWith(key) && cmd.key() != key) {
      return;
    }
  }
  if (!cmdTable_.contains(cmdString)) {
    cmdString = ":quickfix";
    keywords.prepend(cmdString);
  }
  auto commandEmiter = cmdTable_.value(cmdString);
  commandEmiter(cdrList(keywords));
}

void SearchLineEdit::fdCmdEmit(const QStringList &keywords) {
  /// must input at least 3 chars
  if (keywords.isEmpty() || keywords.join("").size() < 3) {
    // setPlaceholderText("at least 3 chars");
    return;
  }
  keywords_ = keywords;
  delayTimer_.start(searchDelay_);
}

void SearchLineEdit::directoryCmdEmit(const QStringList &keywords) {
  clear();
  emit searchKeyWordsChanged(keywords, QtFinder::Cmd::kDirectoryChanged);
}

void SearchLineEdit::quickfixCmdEmit(const QStringList &keywords) {
  emit searchKeyWordsChanged(keywords, QtFinder::Cmd::kQuickfix);
}

void SearchLineEdit::keyPressEvent(QKeyEvent *event) {
  auto key = key_press_event(event);
  if (key != Qt::Key_unknown) {
    emit shortcutKeyPressed(key);
    return;
  }
  QLineEdit::keyPressEvent(event);
}

bool SearchLineEdit::focusNextPrevChild(bool next) { return false; }

static bool isAbsDirOrHome(const QString &key) {
  fs::path dir(key.toStdString());
  if ((fs::exists(dir) && fs::is_directory(dir) && dir.is_absolute()) ||
      (dir == "~")) {
    return true;
  }
  return false;
}
