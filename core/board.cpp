#include "board.h"

#include <QDebug>

Board::Board(QObject *parent) :
    QObject(parent)
{
}

Board::~Board()
{
    QMap<int, BoardChannel *>::const_iterator it;
    for (it = _channels.constBegin(); it != _channels.constEnd(); ++it)
        delete *it;
}

void Board::updateChannels(QMap<int, BoardChannel *> channels)
{
    qDebug() << "BOARD :: channels receved";

    if (!_channels.isEmpty())
    {
        QList<int>::const_iterator it;
        for (it = channels.keys().constBegin();
             it != channels.keys().constEnd(); ++it)
            if (_channels.keys().contains(*it))
                delete _channels[*it];
            _channels.insert(*it, channels[*it]);
    }
    else
        _channels = channels;
    rebuildMessagesTree();
    emit channelsUpdated();
}

void Board::updateMessages(QMap<int, BoardMessage *> messages)
{
    qDebug() << "BOARD :: messages receved";

    if (!_messages.isEmpty())
    {
        QList<int> messagesId = messages.keys();
        QList<int>::const_iterator it;
        for (it = messagesId.constBegin();
             it != messagesId.constEnd(); ++it)
        {
            delete _messages[*it];
            _messages.insert(*it, messages[*it]);
        }
    }
    else
        _messages = messages;
    rebuildMessagesTree();
    emit messagesUpdated();
}

void Board::rebuildMessagesTree()
{
    qDebug() << "BOARD :: rebuild start";
    if (!_messages.isEmpty())
    {
        QList<BoardMessage *> msg = _messages.values();
        QList<BoardMessage *>::const_iterator it;
        for (it = msg.constBegin();
             it != msg.constEnd(); ++it)
        {
            if ((*it)->parentId() == -1)
                (*it)->setParent(getChannel((*it)->channelId()));
            else
                (*it)->setParent(getMessage((*it)->parentId()));
        }
    }
    qDebug() << "BOARD :: rebuild end";
}
