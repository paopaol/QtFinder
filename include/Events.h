#ifndef EVENTS_H
#define EVENTS_H

#include <QKeyEvent>
#include <Qt>

struct ShortcutKey {
  ShortcutKey(Qt::Key key, Qt::KeyboardModifier modifiers, Qt::Key aliaskey)
      : Key(key), Modifier(modifiers), AliasKey(aliaskey) {}

  ShortcutKey(Qt::Key key)
      : Key(key), AliasKey(key), Modifier(Qt::NoModifier) {}
  ShortcutKey(Qt::Key key, Qt::Key aliasKey)
      : Key(key), AliasKey(aliasKey), Modifier(Qt::NoModifier) {}

  Qt::Key aliasKey(QKeyEvent *event) {
    auto key = event->key();
    auto modifiers = event->modifiers();

    return (key == Key && modifiers == Modifier) ? AliasKey : Qt::Key_unknown;
  }

  Qt::Key Key;
  Qt::Key AliasKey;
  Qt::KeyboardModifier Modifier;
};

std::vector<ShortcutKey> &shortcutKeyTable();
Qt::Key key_press_event(QKeyEvent *event);

#endif /* EVENTS_H */
