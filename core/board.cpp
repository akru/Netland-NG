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

#include "board.h"
#include "connector.h"
#include "board_channel.h"
#include "board_message.h"

Board::Board(Connector *conn)
  : QObject(conn), _conn(conn)
{
  connectAll();
}

void Board::connectAll()
{
  connect(_conn, SIGNAL(boardChannelsRecv(QMap<int, shared_ptr<BoardChannel> >)),
          this, SLOT(updateChannels(QMap<int,shared_ptr<BoardChannel> >)));
  connect(_conn, SIGNAL(boardMessagesRecv(QMap<int,shared_ptr<BoardMessage> >)),
          this, SLOT(updateMessages(QMap<int,shared_ptr<BoardMessage> >)));

  connect(this, SIGNAL(addMessageReady(int,QString)),
          _conn, SLOT(boardAddMessage(int,QString,int)));
  connect(this, SIGNAL(addReplyReady(int,QString)),
          _conn, SLOT(boardAddReply(int,QString)));
  connect(this, SIGNAL(editMessageReady(int,QString)),
          _conn, SLOT(boardEditMessage(int,QString,int)));
  connect(this, SIGNAL(deleteMessageReady(int)),
          _conn, SLOT(boardDeleteMessage(int)));
  connect(this, SIGNAL(upMessageReady(int)),
          _conn, SLOT(boardUpMessage(int)));
}

void Board::updateChannels(QMap<int, shared_ptr<BoardChannel> > channels)
{
  _channels = channels;
  rebuildMessagesTree();
  emit channelsUpdated();
}

void Board::updateMessages(QMap<int, shared_ptr<BoardMessage> > messages)
{
  if (!_messages.isEmpty())
  {
    QList<int> messagesId = messages.keys();
    QList<int>::const_iterator it;
    for (it = messagesId.constBegin(); it != messagesId.constEnd(); ++it)
      _messages.insert(*it, messages[*it]);
  }
  else
    _messages = messages;
  rebuildMessagesTree();
  emit messagesUpdated();
}

void Board::rebuildMessagesTree()
{
  if (!_messages.isEmpty())
  {
    QList<shared_ptr<BoardMessage> > msg = _messages.values();
    QList<shared_ptr<BoardMessage> >::const_iterator it;
    for (it = msg.constBegin(); it != msg.constEnd(); ++it)
    {
      if ((*it)->parentId() == -1)
        (*it)->setParent(getChannel((*it)->channelId()).get());
      else
        (*it)->setParent(getMessage((*it)->parentId()).get());
    }
  }
}

void Board::addMessage(int channelId,
                       QString text, int actualityDays)
{
  emit addMessageReady(channelId, text, actualityDays);
}

void Board::addReply(int messageId, QString text)
{
  emit addReplyReady(messageId, text);
}

void Board::editMessage(int messageId,
                        QString text, int actualityDays)
{
  emit editMessageReady(messageId, text, actualityDays);
}

void Board::deleteMessage(int messageId)
{
  emit deleteMessageReady(messageId);
}

void Board::upMessage(int messageId)
{
  emit upMessageReady(messageId);
}
