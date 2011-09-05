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

#ifndef CHAT_CHANNEL_H
#define CHAT_CHANNEL_H

#include <QObject>
#include <QMap>

#include <boost/shared_ptr.hpp>
using namespace boost;

class Connector;
class ChatUser;

class ChatChannel : public QObject
{
  Q_OBJECT
public:
  explicit ChatChannel(Connector *conn,
                       QString id, QString name);
  inline QString id()
  {
    return _id;
  }
  inline QString name()
  {
    return _name;
  }
  inline QList<shared_ptr<ChatUser> > users()
  {
    return _users.values();
  }
  inline shared_ptr<ChatUser> getUser(QString id)
  {
    return _users[id];
  }

signals:
  void usersUpdated();
  void userConnected(shared_ptr<ChatUser> user);
  void userDisconnected(shared_ptr<ChatUser> user);

private:
  void connectAll();

private slots:
  void updateUsers(QString channelId,
                   QMap<QString, shared_ptr<ChatUser> > users);
  void insertUser(QString channelId, shared_ptr<ChatUser> user);
  void removeUser(QString channelId, QString userId);

private:
  Connector *_conn;
  QString _id, _name;
  QMap<QString, shared_ptr<ChatUser> > _users;
};

#endif // CHAT_CHANNEL_H
