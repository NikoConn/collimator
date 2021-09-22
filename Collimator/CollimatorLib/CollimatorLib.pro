QT += core
QT -= gui
QT += network

TEMPLATE = lib
DEFINES += COLLIMATORLIB_LIBRARY

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    collimator.cpp \
    imageresults.cpp

HEADERS += \
    CollimatorLib_global.h \
    collimator.h \
    imageresults.h

include(model/model.pri)

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

#libs
unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv4

QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp
