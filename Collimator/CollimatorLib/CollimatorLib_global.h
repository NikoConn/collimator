#ifndef COLLIMATORLIB_GLOBAL_H
#define COLLIMATORLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(COLLIMATORLIB_LIBRARY)
#  define COLLIMATORLIB_EXPORT Q_DECL_EXPORT
#else
#  define COLLIMATORLIB_EXPORT Q_DECL_IMPORT
#endif

#endif // COLLIMATORLIB_GLOBAL_H
