#include <Events.h>

#define QKEY(key) static_cast<Qt::Key>(key)
static std::vector<ShortcutKey> g_shortcutKeyTable = {
    ShortcutKey(Qt::Key_Tab),

    // default enter directory or of file's directory
    ShortcutKey(Qt::Key_Enter),
    ShortcutKey(Qt::Key_Return),
    ShortcutKey(Qt::Key_Right, Qt::AltModifier, QKEY(Qt::Key_Enter)),

    // default open file
    ShortcutKey(Qt::Key_Enter, Qt::ControlModifier,
                QKEY(Qt::Key_Control | Qt::Key_Enter)),

    ShortcutKey(Qt::Key_Down),
    ShortcutKey(Qt::Key_J, Qt::ControlModifier, QKEY(Qt::Key_Down)),
    ShortcutKey(Qt::Key_N, Qt::ControlModifier, QKEY(Qt::Key_Down)),

    ShortcutKey(Qt::Key_Up),
    ShortcutKey(Qt::Key_P, Qt::ControlModifier, QKEY(Qt::Key_Up)),
    ShortcutKey(Qt::Key_K, Qt::ControlModifier, QKEY(Qt::Key_Up)),

    // cd up
    ShortcutKey(Qt::Key_H, Qt::ControlModifier,
                QKEY(Qt::Key_Control | Qt::Key_H)),
    ShortcutKey(Qt::Key_Left, Qt::AltModifier,
                QKEY(Qt::Key_Control | Qt::Key_H)),

};

std::vector<ShortcutKey> &shortcutKeyTable() { return g_shortcutKeyTable; }

Qt::Key key_press_event(QKeyEvent *event) {
  for (auto &shortcutKey : shortcutKeyTable()) {
    Qt::Key key = shortcutKey.aliasKey(event);
    if (key != Qt::Key_unknown) {
      return key;
    }
  }
  return Qt::Key_unknown;
}
