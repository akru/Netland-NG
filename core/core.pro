#
#      This file is part of the Netland-NG.
#
#      This program is free software; you can redistribute it and/or modify
#      it under the terms of the GNU General Public License as published by
#      the Free Software Foundation; either version 2 of the License, or
#      (at your option) any later version.
#
#      This program is distributed in the hope that it will be useful,
#      but WITHOUT ANY WARRANTY; without even the implied warranty of
#      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#      GNU General Public License for more details.
#
#      You should have received a copy of the GNU General Public License
#      along with this program; if not, write to the Free Software
#      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#      MA 02110-1301, USA.
#

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
