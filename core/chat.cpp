#include "chat.h"
#include "chat_channel.h"
#include "chat_user.h"
#include "chat_private.h"

Chat::Chat(QObject *parent) :
    QObject(parent)
{
}

void Chat::updateChannels(QMap<QString, shared_ptr<ChatChannel> > channels)
{
    _channels = channels;
    emit channelsUpdated();
}
