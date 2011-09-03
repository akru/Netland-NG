#-------------------------------------------------
#
# Project created by QtCreator 2011-08-24T18:02:00
#
#-------------------------------------------------

QT       += core network
QT       -= gui

TEMPLATE = lib

TARGET = core
CONFIG   += staticlib
CONFIG   -= app_bundle

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += \
    encode.cpp \
    connector.cpp \
    connector_old.cpp \
    board.cpp \
    chat.cpp \
    board_channel.cpp \
    board_message.cpp \
    chat_channel.cpp \
    chat_user.cpp \
    chat_private.cpp

HEADERS += \
    encode.h \
    connector.h \
    connector_old.h \
    board.h \
    chat.h \
    board_channel.h \
    board_message.h \
    chat_channel.h \
    chat_user.h \
    chat_private.h
