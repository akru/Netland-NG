#include "main_application.h"

#include <connector_old.h>
#include <board_channel.h>
#include <board_message.h>

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
}

// Tests

void MainApplication::testAuthentification()
{
    qDebug() << "Authentification success!";
    conn->setNick("inspier");
}

void MainApplication::testViewChannels()
{
    qDebug() << "View channels:";

    QList<BoardChannel *> channels = conn->getBoardInstance()->channels();
    QList<BoardChannel *>::const_iterator it;
    for (it = channels.constBegin();
         it != channels.constEnd(); ++it)
    {
        qDebug() << "--> " << (*it)->name() << " :: " << (*it)->description();
    }
}

void MainApplication::testViewMessages()
{
    qDebug() << "View messages:";
    BoardChannel *ch = conn->getBoardInstance()->channels().at(0);
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
