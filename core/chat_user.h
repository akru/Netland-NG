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

#ifndef CHAT_USER_H
#define CHAT_USER_H

#include <QObject>
#include <QMap>
#include <QDateTime>

#include <boost/shared_ptr.hpp>
using namespace boost;

class Connector;
class ChatPrivate;

class ChatUser : public QObject
{
  Q_OBJECT
public:
  explicit ChatUser(Connector *conn,
                    QString channelId, QString id, QString nick,
                    QString ip, QString computerName);
  inline QString id()
  {
    return _id;
  }
  inline QString channelId()
  {
    return _channel_id;
  }
  inline QString nick()
  {
    return _nick;
  }
  inline QString ip()
  {
    return _ip;
  }
  inline QString computerName()
  {
    return _computer_name;
  }
  inline QMap<QDateTime, shared_ptr<ChatPrivate> > history()
  {
    return _history;
  }
  inline shared_ptr<ChatPrivate> getPrivate(QString id)
  {
    return _messages[id];
  }

signals:
  void newPrivate(QString text);
  void sendPrivateReady(shared_ptr<ChatPrivate> msg);
  void privateDelivered(shared_ptr<ChatPrivate> msg);

public slots:
  void sendPrivate(QString text);

private slots:
  void recvPrivate(shared_ptr<ChatPrivate> msg);
  void recvDeliveredReport(QString channelId,
                           QString userId, QString messageId);

private:
  void connectAll();

private:
  Connector *_conn;
  QString _channel_id, _id, _nick, _ip, _computer_name;
  QMap<QDateTime, shared_ptr<ChatPrivate> > _history;
  QMap<QString, shared_ptr<ChatPrivate> > _messages;
};

#endif // CHAT_USER_H
