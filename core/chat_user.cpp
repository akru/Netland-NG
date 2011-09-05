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

#include "chat_user.h"
#include "connector.h"
#include "chat_private.h"

ChatUser::ChatUser(Connector *conn,
                   QString channelId, QString id, QString nick,
                   QString ip, QString computerName)
  : QObject(conn),
    _conn(conn), _channel_id(channelId), _id(id), _nick(nick),
    _ip(ip), _computer_name(computerName)
{
  connectAll();
}

void ChatUser::connectAll()
{
  connect(_conn, SIGNAL(chatPrivateMessage(shared_ptr<ChatPrivate>)),
          this, SLOT(recvPrivate(shared_ptr<ChatPrivate>)));
  connect(this, SIGNAL(sendPrivateReady(shared_ptr<ChatPrivate>)),
          _conn, SLOT(chatSendPrivate(shared_ptr<ChatPrivate>)));
  connect(_conn, SIGNAL(chatPrivateDelivered(QString,QString,QString)),
          this, SLOT(recvDeliveredReport(QString,QString,QString)));
}

void ChatUser::recvPrivate(shared_ptr<ChatPrivate> msg)
{
  if (msg->senderId() == _id)
  {
    _history.insert(msg->created(), msg);
    emit newPrivate(msg->text());
  }
}

void ChatUser::recvDeliveredReport(QString channelId,
                                   QString userId, QString messageId)
{
  if (_channel_id == channelId && _id == userId)
  {
    getPrivate(messageId)->setDelivered(
          QDateTime::currentDateTime());
    emit privateDelivered(getPrivate(messageId));
  }
}

void ChatUser::sendPrivate(QString text)
{
  shared_ptr<ChatPrivate> msg(
        new ChatPrivate(
          _channel_id,
          "",
          _id,
          text));
  _history.insert(msg->created(), msg);
  _messages.insert(msg->text(), msg);
  emit sendPrivateReady(msg);
}
