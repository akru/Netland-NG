#include "main_application.h"

#include <connector_old.h>
#include <board.h>
#include <board_channel.h>
#include <board_message.h>
#include <chat.h>
#include <chat_channel.h>
#include <chat_user.h>

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
            this, SLOT(testViewChannels()));
    connect(conn->getBoardInstance(), SIGNAL(messagesUpdated()),
            this, SLOT(testViewMessages()));
    connect(conn->getChatInstance(), SIGNAL(usersUpdated(QString)),
            this, SLOT(testViewChat(QString)));
    connect(conn->getChatInstance(), SIGNAL(userConnected(shared_ptr<ChatUser>)),
            this, SLOT(testChatUCon(shared_ptr<ChatUser>)));
    connect(conn->getChatInstance(), SIGNAL(userDisconnected(shared_ptr<ChatUser>)),
            this, SLOT(testChatUDiscon(shared_ptr<ChatUser>)));
}

// Tests

void MainApplication::testAuthentification()
{
    qDebug() << "Authentification success!";
    conn->setNick("inspier");
}

void MainApplication::testViewChannels()
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

void MainApplication::testViewMessages()
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

void MainApplication::testViewChat(QString channelId)
{
    shared_ptr<ChatChannel> ch = conn->getChatInstance()->getChannel(channelId);
    qDebug() << ch->name() << "::" <<
                    ch->users().count() << "users";
}

void MainApplication::testChatUCon(shared_ptr<ChatUser> user)
{
    qDebug() << "Chat:" << user->nick() << "connected";
}

void MainApplication::testChatUDiscon(shared_ptr<ChatUser> user)
{
    qDebug() << "Chat:" << user->nick() << "disconnected";
}
