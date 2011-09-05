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

#ifndef CHAT_H
#define CHAT_H

#include <QObject>
#include <QMap>

#include <boost/shared_ptr.hpp>
using namespace boost;

class ChatChannel;
class ChatUser;
class ChatPrivate;
class Connector;

class Chat
    : public QObject
{
  Q_OBJECT
public:
  explicit Chat(Connector *conn);
  inline QList<shared_ptr<ChatChannel> > channels()
  {
    return _channels.values();
  }
  inline shared_ptr<ChatChannel> getChannel(QString id)
  {
    return _channels[id];
  }
  shared_ptr<ChatChannel> getChannel(QString id, bool *ok);

signals:
  void channelsUpdated();

private slots:
  void updateChannels(QMap<QString, shared_ptr<ChatChannel> > channels);

private:
  void connectAll();

private:
  Connector *_conn;
  QMap<QString, shared_ptr<ChatChannel> > _channels;
};

#endif // CHAT_H
