#include <Events.h>

Qt::Key key_press_event(QKeyEvent *event) {
  switch (event->key()) {
  case Qt::Key_Tab: {
    return Qt::Key_Tab;
  }
  case Qt::Key_Enter:
  case Qt::Key_Return: {
    if (event->modifiers() == Qt::ControlModifier) {
      return static_cast<Qt::Key>(Qt::Key_Control | Qt::Key_Enter);
    }
    return Qt::Key_Enter;
  }
  case Qt::Key_Down: {
    return Qt::Key_Down;
  }
  case Qt::Key_N:
  case Qt::Key_J: {
    if (event->modifiers() == Qt::ControlModifier) {
      return Qt::Key_Down;
    }
    break;
  }
  case Qt::Key_Up: {
    return Qt::Key_Up;
  }
  case Qt::Key_P:
  case Qt::Key_K: {
    if (event->modifiers() == Qt::ControlModifier) {
      return Qt::Key_Up;
    }
    break;
  }
  default:
    break;
  }
  return Qt::Key_unknown;
}
