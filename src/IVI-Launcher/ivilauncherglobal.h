#ifndef IVILAUNCHERGLOBAL_H
#define IVILAUNCHERGLOBAL_H

#include <QtCore/qglobal.h>

QT_BEGIN_NAMESPACE

#ifndef Q_IVI_LAUNCHER_EXPORT
#  ifndef QT_STATIC
#    if defined(QT_BUILD_IVI_LAUNCHER_LIB)
#      define Q_IVI_LAUNCHER_EXPORT Q_DECL_EXPORT
#    else
#      define Q_IVI_LAUNCHER_EXPORT Q_DECL_IMPORT
#    endif
#  else
#    define Q_IVI_LAUNCHER_EXPORT
#  endif
#endif

QT_END_NAMESPACE

#endif // IVILAUNCHERGLOBAL_H
