/*
 *      This file is part of the Netland-NG.
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 *
 */

#include "connector.h"
#include "board.h"
#include "chat.h"
#include <QHostAddress>

Connector::Connector(QObject *parent) :
    QTcpSocket(parent),
    _board(new Board(this)), _chat(new Chat(this))
{
    connectAll();
}

void Connector::connectAll()
{
    // === Board functional === //
    connect(this, SIGNAL(boardChannelsRecv(QMap<int, shared_ptr<BoardChannel> >)),
            _board, SLOT(updateChannels(QMap<int,shared_ptr<BoardChannel> >)));
    connect(_board, SIGNAL(channelsUpdated()),
            this, SLOT(boardUpdateMessages()));
    connect(this, SIGNAL(boardMessagesRecv(QMap<int,shared_ptr<BoardMessage> >)),
            _board, SLOT(updateMessages(QMap<int,shared_ptr<BoardMessage> >)));
    connect(this, SIGNAL(boardNewMessages()),
            this, SLOT(boardUpdateMessages()));

    // === Chat functional  === //
    connect(this, SIGNAL(nickIsSet()), this, SLOT(chatSetNick()));
    connect(this, SIGNAL(chatChannelsRecv(QMap<QString,shared_ptr<ChatChannel> >)),
            _chat, SLOT(updateChannels(QMap<QString,shared_ptr<ChatChannel> >)));
    connect(_chat, SIGNAL(channelsUpdated()),
            this, SLOT(chatUpdateUsers()));
}

void Connector::setNick(QString nick)
{
    _nick = nick;
    emit nickIsSet();
}

void Connector::connectToServer(QString address, QString port)
{
    connectToHost(QHostAddress(address), port.toInt());
}
