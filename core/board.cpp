#include "board.h"
#include "connector.h"
#include "board_channel.h"
#include "board_message.h"

Board::Board(Connector *conn)
  : QObject(conn), parent(conn)
{
}

void Board::updateChannels(QMap<int, shared_ptr<BoardChannel> > channels)
{
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
    parent->boardAddMessage(channelId, text, actualityDays);
}

void Board::addReply(int messageId, QString text)
{
    parent->boardAddReply(messageId, text);
}

void Board::editMessage(int messageId,
                        QString text, int actualityDays)
{
    parent->boardEditMessage(messageId, text, actualityDays);
}

void Board::deleteMessage(int messageId)
{
    parent->boardDeleteMessage(messageId);
}

void Board::upMessage(int messageId)
{
    parent->boardUpMessage(messageId);
}
