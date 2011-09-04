#include "board.h"

Board::Board(QObject *parent) :
    QObject(parent)
{
}

void Board::updateChannels(QMap<int, shared_ptr<BoardChannel> > channels)
{
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

void Board::updateMessages(QMap<int, shared_ptr<BoardMessage> > messages)
{
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
    if (!_messages.isEmpty())
    {
        QList<shared_ptr<BoardMessage> > msg = _messages.values();
        QList<shared_ptr<BoardMessage> >::const_iterator it;
        for (it = msg.constBegin();
             it != msg.constEnd(); ++it)
        {
            if ((*it)->parentId() == -1)
                (*it)->setParent(getChannel((*it)->channelId()).get());
            else
                (*it)->setParent(getMessage((*it)->parentId()).get());
        }
    }
}

void Board::addMessage(int channelId,
                       QString text, int actualityDays)
{
    emit doAddMessage(channelId, text, actualityDays);
}

void Board::addReply(int messageId, QString text)
{
    emit doAddReply(messageId, text);
}

void Board::editMessage(int messageId,
                        QString text, int actualityDays)
{
    emit doEditMessage(messageId, text, actualityDays);
}

void Board::deleteMessage(int messageId)
{
    emit doDeleteMessage(messageId);
}

void Board::upMessage(int messageId)
{
    emit doUpMessage(messageId);
}
