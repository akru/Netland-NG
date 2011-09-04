#include "chat.h"
#include "chat_channel.h"
#include "chat_user.h"

Chat::Chat(QObject *parent) :
    QObject(parent)
{
}

void Chat::updateChannels(QMap<QString, shared_ptr<ChatChannel> > channels)
{
    _channels = channels;
    emit channelsUpdated();
}

void Chat::updateUsers(QString channelId,
                       QMap<QString, shared_ptr<ChatUser> > users)
{
    getChannel(channelId)->updateUsers(users);
    emit usersUpdated(channelId);
}

void Chat::insertUser(QString channelId, shared_ptr<ChatUser> user)
{
    emit userConnected(user);
    getChannel(channelId)->insertUser(user);
}

void Chat::removeUser(QString channelId, QString userId)
{
    emit userDisconnected(
                getChannel(channelId)->getUser(userId));
    getChannel(channelId)->removeUser(userId);
}
