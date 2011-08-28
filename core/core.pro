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
    connector_old.cpp \
    board.cpp \
    chat.cpp \
    board_channel.cpp \
    board_message.cpp \
    main_application.cpp

HEADERS += \
    encode.h \
    connector.h \
    connector_old.h \
    board.h \
    chat.h \
    board_channel.h \
    board_message.h \
    main_application.h
