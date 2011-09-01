#include "connector.h"

#include <QHostAddress>

Connector::Connector(QObject *parent) :
    QTcpSocket(parent),
    _board(new Board(this)), _chat(new Chat(this))
{
    connectAll();
}

void Connector::connectAll()
{
    // Board functional
    connect(this, SIGNAL(boardChannelsRecv(QMap<int,BoardChannel*>)),
            _board, SLOT(updateChannels(QMap<int,BoardChannel*>)));
    connect(_board, SIGNAL(channelsUpdated()),
            this, SLOT(boardUpdateMessages()));
    connect(this, SIGNAL(boardMessagesRecv(QMap<int,BoardMessage*>)),
            _board, SLOT(updateMessages(QMap<int,BoardMessage*>)));
    connect(this, SIGNAL(boardNewMessages()),
            this, SLOT(boardUpdateMessages()));
    connect(_board, SIGNAL(doAddMessage(BoardChannel*,QString,int)),
            this, SLOT(boardAddMessage(BoardChannel*,QString,int)));
    connect(_board, SIGNAL(doAddReply(BoardMessage*,QString)),
            this, SLOT(boardAddReply(BoardMessage*,QString)));
    connect(_board, SIGNAL(doEditMessage(BoardMessage*,QString,int)),
            this, SLOT(boardEditMessage(BoardMessage*,QString,int)));
    connect(_board, SIGNAL(doDeleteMessage(BoardMessage*)),
            this, SLOT(boardDeleteMessage(BoardMessage*)));
    connect(_board, SIGNAL(doUpMessage(BoardMessage*)),
            this, SLOT(boardUpMessage(BoardMessage*)));
    // Chat functional
    connect(this, SIGNAL(nickIsSet()), this, SLOT(chatSetNick()));
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
