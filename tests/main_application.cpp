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

#include "main_application.h"
#include <connector_old.h>
#include <board.h>
#include <board_channel.h>
#include <board_message.h>
#include <chat.h>
#include <chat_channel.h>
#include <chat_user.h>
#include <chat_private.h>

MainApplication::MainApplication(int argc, char *argv[])
  : QCoreApplication(argc, argv),
    conn(new ConnectorOld)
{
  connectAll();
  conn->connectToServer("192.168.111.210", "1539");
}

MainApplication::~MainApplication()
{
  delete conn;
}

void MainApplication::connectAll()
{
  connect(conn, SIGNAL(authSuccess()),
          this, SLOT(testAuthentification()));
  connect(conn->getBoardInstance(), SIGNAL(channelsUpdated()),
          this, SLOT(testBoardViewChannels()));
  connect(conn->getBoardInstance(), SIGNAL(messagesUpdated()),
          this, SLOT(testBoardViewMessages()));
  connect(conn->getChatInstance(), SIGNAL(channelsUpdated()),
          this, SLOT(testChatViewChannels()));
  connect(conn, SIGNAL(chatPrivateMessage(shared_ptr<ChatPrivate>)),
          this, SLOT(testChatPrivateMessage(shared_ptr<ChatPrivate>)));
}

// Tests

void MainApplication::testAuthentification()
{
  qDebug() << "Authentification success!";
  conn->setNick("inspier");
}

void MainApplication::testBoardViewChannels()
{
  qDebug() << "View board channels:";
  QList<shared_ptr<BoardChannel> > channels = conn->getBoardInstance()->channels();
  QList<shared_ptr<BoardChannel> >::const_iterator it;
  for (it = channels.constBegin(); it != channels.constEnd(); ++it)
  {
    qDebug() << "--> " << (*it)->name() << " :: " << (*it)->description();
  }
}

void MainApplication::testBoardViewMessages()
{
  qDebug() << "View board messages:";
  shared_ptr<BoardChannel> ch = conn->getBoardInstance()->channels().at(0);
  qDebug() << ch->name() << ":";

  QObjectList messages = ch->children();
  QObjectList::const_iterator it;
  for (it = messages.constBegin(); it != messages.constEnd(); ++it)
  {
    qDebug() << ((BoardMessage *) *it)->nick()
             << "@"
             <<  ((BoardMessage *) *it)->hostname()
             << ":\n" << ((BoardMessage *) *it)->body().left(15) << "..."
             << ((BoardMessage *) *it)->editTime() << "\n + ["
             << ((BoardMessage *) *it)->children().count() << "]\n\n";
  }
}

void MainApplication::testChatViewChannels()
{
  qDebug() << "View chat channels:";
  QList<shared_ptr<ChatChannel> > channels =
      conn->getChatInstance()->channels();
  QList<shared_ptr<ChatChannel> >::const_iterator it;
  for (it = channels.constBegin();
       it != channels.constEnd(); ++it)
  {
    qDebug() << (*it)->name();
    connect((*it).get(), SIGNAL(usersUpdated()),
            this, SLOT(testChatViewChannelUsers()));
    connect((*it).get(), SIGNAL(userConnected(shared_ptr<ChatUser>)),
            this, SLOT(testChatViewConnected(shared_ptr<ChatUser>)));
    connect((*it).get(), SIGNAL(userDisconnected(shared_ptr<ChatUser>)),
            this, SLOT(testChatViewDisconnected(shared_ptr<ChatUser>)));
    connect((*it).get(), SIGNAL(publicMessage(shared_ptr<ChatUser>,QString)),
            this, SLOT(testChatPublicMessage(shared_ptr<ChatUser>,QString)));
  }
}

void MainApplication::testChatViewChannelUsers()
{
  QList<shared_ptr<ChatChannel> > channels =
      conn->getChatInstance()->channels();
  QList<shared_ptr<ChatChannel> >::const_iterator it;
  for (it = channels.constBegin();
       it != channels.constEnd(); ++it)
  {
    qDebug() << (*it)->name() << "::" <<
                    (*it)->users().count() << "users";
  }
}

void MainApplication::testChatViewConnected(shared_ptr<ChatUser> user)
{
  qDebug() << "Chat:" << user->nick()<< "connected";
}

void MainApplication::testChatViewDisconnected(shared_ptr<ChatUser> user)
{
  qDebug() << "Chat:" << user->nick() << "disconnected";
}

void MainApplication::testChatPrivateMessage(shared_ptr<ChatPrivate> msg)
{
  Chat *chat = conn->getChatInstance();
  shared_ptr<ChatUser> sndr =
      chat->getChannel(msg->channelId())->getUser(msg->senderId());
  qDebug() << "Private:" << sndr->nick() << msg->text();
  sndr->sendPrivate(msg->text());
  connect(sndr.get(), SIGNAL(privateDelivered(shared_ptr<ChatPrivate>)),
          this, SLOT(testChatDeliveredReport(shared_ptr<ChatPrivate>)));
}

void MainApplication::testChatDeliveredReport(shared_ptr<ChatPrivate> msg)
{
  Chat *chat = conn->getChatInstance();
  shared_ptr<ChatUser> sndr =
      chat->getChannel(msg->channelId())->getUser(msg->receiverId());
  qDebug() << "Private to" << sndr->nick()
           << ":" << msg->text() << "delivered" << msg->delivered();
  disconnect(this, SLOT(testChatDeliveredReport(shared_ptr<ChatPrivate>)));
}

void MainApplication::testChatPublicMessage(shared_ptr<ChatUser> user, QString text)
{
  qDebug() << "Chat: [" << user->channelId() << "]:" << text;
  if (text == ";")
    conn->getChatInstance()->getChannel(
          user->channelId())->sendPublic(",");
}
