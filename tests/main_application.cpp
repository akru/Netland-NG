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
    for (it = channels.constBegin();
         it != channels.constEnd(); ++it)
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
        qDebug() << ((BoardMessage *) *it)->nick() << "@" <<  ((BoardMessage *) *it)->hostname()
                 << " ::  " << ((BoardMessage *) *it)->body() <<
                    ((BoardMessage *) *it)->editTime() << "\n + [" <<
                    ((BoardMessage *) *it)->children().count() << "]\n\n";
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
    qDebug() << "Chat:" << user->nick() << "connected";
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
}
