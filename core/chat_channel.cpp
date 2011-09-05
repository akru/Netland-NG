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

#include "chat_channel.h"
#include "chat_user.h"
#include "connector.h"

ChatChannel::ChatChannel(Connector *conn, QString id, QString name)
  : QObject(conn),
    _conn(conn), _id (id), _name(name)
{
  connectAll();
}

void ChatChannel::connectAll()
{
  connect(_conn, SIGNAL(chatUsersRecv(QString,QMap<QString,shared_ptr<ChatUser> >)),
          this, SLOT(updateUsers(QString,QMap<QString,shared_ptr<ChatUser> >)));
  connect(_conn, SIGNAL(chatUserEnter(QString,shared_ptr<ChatUser>)),
          this, SLOT(insertUser(QString,shared_ptr<ChatUser>)));
  connect(_conn, SIGNAL(chatUserLeave(QString,QString)),
          this, SLOT(removeUser(QString,QString)));
}

shared_ptr<ChatUser>
ChatChannel::getUser(QString id, bool *ok)
{
  if (_users.keys().contains(id))
  {
    *ok = true;
    return _users[id];
  }
  else
  {
    *ok = false;
    return shared_ptr<ChatUser>();
  }
}

void ChatChannel::updateUsers(QString channelId,
                              QMap<QString, shared_ptr<ChatUser> > users)
{
  if (channelId == _id)
  {
    _users = users;
    emit usersUpdated();
  }
}

void ChatChannel::insertUser(QString channelId, shared_ptr<ChatUser> user)
{
  if (channelId == _id)
  {
    _users.insert(user->id(), user);
    emit userConnected(user);
    emit usersUpdated();
  }
}

void ChatChannel::removeUser(QString channelId, QString userId)
{
  if (channelId == _id)
  {
    emit userDisconnected(getUser(userId));
    _users.remove(userId);
    emit usersUpdated();
  }
}

