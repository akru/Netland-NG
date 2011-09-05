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

#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QMap>

#include <boost/shared_ptr.hpp>
using namespace boost;

class BoardChannel;
class BoardMessage;
class Connector;

class Board
    : public QObject
{
  Q_OBJECT
public:
  explicit Board(Connector *conn);
  inline QList<shared_ptr<BoardChannel> > channels()
  {
    return _channels.values();
  }
  inline shared_ptr<BoardChannel> getChannel(int id)
  {
    return _channels[id];
  }
  inline shared_ptr<BoardMessage> getMessage(int id)
  {
    return _messages[id];
  }

signals:
    void channelsUpdated();
    void messagesUpdated();

public slots:
    void addMessage(int channelId,
                    QString text, int actualityDays = 30);
    void addReply(int messageId, QString text);
    void editMessage(int messageId,
                     QString text, int actualityDays = 30);
    void deleteMessage(int messageId);
    void upMessage(int messageId);

private slots:
    void updateChannels(QMap<int, shared_ptr<BoardChannel> > channels);
    void updateMessages(QMap<int, shared_ptr<BoardMessage> > messages);

private:
    void rebuildMessagesTree();

private:
    Connector *parent;
    QMap<int, shared_ptr<BoardChannel> > _channels;
    QMap<int, shared_ptr<BoardMessage> > _messages;
};

#endif // BOARD_H
