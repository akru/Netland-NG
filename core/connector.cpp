#include "connector.h"

Connector::Connector(QObject *parent) :
    QTcpSocket(parent),
    _board(new Board), _chat(new Chat)
{
    connectAll();
}

Connector::~Connector()
{
    delete _board;
    delete _chat;
}

void Connector::connectAll()
{
    connect(this, SIGNAL(boardChannelsRecv(QMap<int,BoardChannel*>)),
            _board, SLOT(updateChannels(QMap<int,BoardChannel*>)));
    connect(_board, SIGNAL(channelsUpdated()),
            this, SLOT(boardUpdateMessages()));
    connect(this, SIGNAL(boardMessagesRecv(QMap<int,BoardMessage*>)),
            _board, SLOT(updateMessages(QMap<int,BoardMessage*>)));
    connect(this, SIGNAL(boardNewMessages()),
            this, SLOT(boardUpdateMessages()));
}
