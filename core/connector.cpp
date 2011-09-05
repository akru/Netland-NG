#include "connector.h"
#include "board.h"
#include "chat.h"
#include <QHostAddress>

Connector::Connector(QObject *parent) :
    QTcpSocket(parent),
    _board(new Board(this)), _chat(new Chat(this))
{
    connectAll();
}

void Connector::connectAll()
{
    // === Board functional === //
    connect(this, SIGNAL(boardChannelsRecv(QMap<int, shared_ptr<BoardChannel> >)),
            _board, SLOT(updateChannels(QMap<int,shared_ptr<BoardChannel> >)));
    connect(_board, SIGNAL(channelsUpdated()),
            this, SLOT(boardUpdateMessages()));
    connect(this, SIGNAL(boardMessagesRecv(QMap<int,shared_ptr<BoardMessage> >)),
            _board, SLOT(updateMessages(QMap<int,shared_ptr<BoardMessage> >)));
    connect(this, SIGNAL(boardNewMessages()),
            this, SLOT(boardUpdateMessages()));

    // === Chat functional  === //
    connect(this, SIGNAL(nickIsSet()), this, SLOT(chatSetNick()));
    connect(this, SIGNAL(chatChannelsRecv(QMap<QString,shared_ptr<ChatChannel> >)),
            _chat, SLOT(updateChannels(QMap<QString,shared_ptr<ChatChannel> >)));
    connect(_chat, SIGNAL(channelsUpdated()),
            this, SLOT(chatUpdateUsers()));
}

void Connector::setNick(QString nick)
{
    _nick = nick;
    emit nickIsSet();
}

void Connector::connectToServer(QString address, QString port)
{
    connectToHost(QHostAddress(address), port.toInt());
}
