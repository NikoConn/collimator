TEMPLATE = app

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

include(persistence/persistence.pri)
include(views/views.pri)
include(logic/logic.pri)

SOURCES += \
    main.cpp \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QT += sql

RESOURCES += \
    Resources.qrc

#libs
unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv4

LIBS += -lopencv_imgproc -lopencv_highgui -lopencv_core
LIBS += -lindiclient -lz
LIBS += -lcfitsio

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../CollimatorLib/release/ -lCollimatorLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../CollimatorLib/debug/ -lCollimatorLib
else:unix: LIBS += -L$$OUT_PWD/../CollimatorLib/ -lCollimatorLib

INCLUDEPATH += $$PWD/../CollimatorLib
DEPENDPATH += $$PWD/../CollimatorLib
