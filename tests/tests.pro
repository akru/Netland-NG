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

INCLUDEPATH += ../core
LIBS += -L../core -lcore

TEMPLATE = app

SOURCES += main.cpp \
    main_application.cpp

HEADERS += \
    main_application.h
