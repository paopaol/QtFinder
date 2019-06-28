#ifndef QTFINDERCMD_H
#define QTFINDERCMD_H

#include <QMetaType>

namespace QtFinder {
enum class Cmd { kQuickfix, kFd, kDirectoryChanged };
}

Q_DECLARE_METATYPE(QtFinder::Cmd)
#endif /* QTFINDERCMD_H */
