#include "board.h"

#include <QDebug>

Board::Board(QObject *parent) :
    QObject(parent)
{
}

Board::~Board()
{
}

void Board::updateChannels(QMap<int, BoardChannel *> channels)
{
    qDebug() << "BOARD :: channels receved";

    if (!_channels.isEmpty())
    {
        QList<int> channelsId = channels.keys();
        QList<int>::const_iterator it;
        for (it = channelsId.constBegin();
             it != channelsId.constEnd(); ++it)
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
            _messages.insert(*it, messages[*it]);
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

void Board::addMessage(BoardChannel *channel,
                       QString text, int actualityDays)
{
    emit doAddMessage(channel, text, actualityDays);
}

void Board::addReply(BoardMessage *message, QString text)
{
    emit doAddReply(message, text);
}

void Board::editMessage(BoardMessage *message,
                        QString text, int actualityDays)
{
    emit doEditMessage(message, text, actualityDays);
}

void Board::deleteMessage(BoardMessage *message)
{
    emit doDeleteMessage(message);
}

void Board::upMessage(BoardMessage *message)
{
    emit doUpMessage(message);
}
