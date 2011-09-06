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

#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QTcpSocket>
#include <QMap>

#include <boost/shared_ptr.hpp>
using namespace boost;

class Board;
class BoardChannel;
class BoardMessage;
class Chat;
class ChatChannel;
class ChatUser;
class ChatPrivate;

class Connector
    : public QTcpSocket
{
  Q_OBJECT
public:
  Connector(QObject *parent = 0);
  inline Board * getBoardInstance()
  {
    return _board;
  }
  inline Chat * getChatInstance()
  {
    return _chat;
  }

signals:
  // === Authentification === //
  void authSuccess();

  // === Information      === //
  void infoMessage(QString msg);

  // === Board            === //
  void boardChannelsRecv(QMap<int, shared_ptr<BoardChannel> > channels);
  void boardMessagesRecv(QMap<int, shared_ptr<BoardMessage> > messages);
  void boardNewMessages();

  // === Chat             === //
  void chatChannelsRecv(QMap<QString, shared_ptr<ChatChannel> > channels);
  void chatUsersRecv(QString channelId,
                     QMap<QString, shared_ptr<ChatUser> > users);
  void chatUserEnter(QString channelId, shared_ptr<ChatUser> user);
  void chatUserLeave(QString channelId, QString userId);
  void chatPrivateMessage(shared_ptr<ChatPrivate> message);
  void chatPrivateDelivered(QString channelId, QString userId, QString messageId);
  void chatPublicMessage(QString channelId, QString userId, QString text);

  // === Common           === //
  void nickIsSet();

public slots:
  // === Common          === //
  void setNick(QString nick);
  void connectToServer(QString address, QString port);

  // === Board           === //
  virtual void boardUpdateMessages() = 0;
  virtual void boardAddMessage(int channelId,
                               QString text, int actualityDays) = 0;
  virtual void boardAddReply(int messageId, QString text) = 0;
  virtual void boardEditMessage(int messageId,
                                QString text, int actualityDays) = 0;
  virtual void boardDeleteMessage(int messageId) = 0;
  virtual void boardUpMessage(int messageId) = 0;

  // === Chat            === //
  virtual void chatUpdateUsers() = 0;
  virtual void chatSendPrivate(shared_ptr<ChatPrivate> msg) = 0;
  virtual void chatSendPublic(QString channelId, QString text) = 0;

private slots:
  // === Chat            === //
  virtual void chatSetNick() = 0;

protected:
  QString _nick;

private:
  void connectAll();

private:
  Board *_board;
  Chat *_chat;
};

#endif // CONNECTOR_H
