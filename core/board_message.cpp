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

#include "board_message.h"
#include "connector.h"

BoardMessage::BoardMessage(Connector *conn,
                           int id, int parent_id, int channel_id, int time_id,
                           int deleted, QString ip, QString mac, QString hostname,
                           QString nick, QString body, QDateTime valid_time,
                           QDateTime post_time, QDateTime edit_time)
    : _conn(conn), _id(id), _parent_id(parent_id), _channel_id(channel_id),
      _time_id(time_id), _deleted(deleted), _ip(ip), _mac(mac),
      _hostname(hostname), _nick(nick), _body(body), _valid_time(valid_time),
      _post_time(post_time), _edit_time(edit_time)
{
}

void BoardMessage::addReply(QString text)
{
  _conn->boardAddReply(_id, text);
}

void BoardMessage::editMessage(QString text, int actualityDays)
{
  _conn->boardEditMessage(_id, text, actualityDays);
}

void BoardMessage::deleteMessage()
{
  _conn->boardDeleteMessage(_id);
}

void BoardMessage::upMessage()
{
  _conn->boardUpMessage(_id);
}
