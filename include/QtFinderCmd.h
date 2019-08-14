#ifndef QTFINDERCMD_H
#define QTFINDERCMD_H

#include <QMap>
#include <QMetaType>
#include <QString>

namespace QtFinder {
enum class Cmd { kQuickfix, kFd, kDirectoryChanged };
inline QString toQString(Cmd cmd) {
  static const QMap<Cmd, QString> table = {
      {Cmd::kFd, ":fd"},
      {Cmd::kDirectoryChanged, ":directory"},
      {Cmd::kQuickfix, ":quickfix"}};
  return table[cmd];
}
// namespace Cmd
} // namespace QtFinder

Q_DECLARE_METATYPE(QtFinder::Cmd)
#endif /* QTFINDERCMD_H */
