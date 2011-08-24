#-------------------------------------------------
#
# Project created by QtCreator 2011-08-24T18:02:00
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = core
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    encode.cpp \
    connector.cpp \
    connector_old.cpp

HEADERS += \
    encode.h \
    connector.h \
    connector_old.h
