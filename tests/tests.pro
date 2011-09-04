#-------------------------------------------------
#
# Project created by QtCreator 2011-08-31T05:40:05
#
#-------------------------------------------------

QT       += core network
QT       -= gui

TARGET = tests
CONFIG   += console
CONFIG   -= app_bundle

QMAKE_CXXFLAGS += -std=c++0x

TEMPLATE = app

SOURCES += main.cpp \
    main_application.cpp

HEADERS += \
    main_application.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../core/release/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../core/debug/ -lcore
else:unix:!symbian: LIBS += -L$$OUT_PWD/../core/ -lcore

INCLUDEPATH += $$PWD/../core
DEPENDPATH += $$PWD/../core

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/core.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/core.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../core/libcore.a
