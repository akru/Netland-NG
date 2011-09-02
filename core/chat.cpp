#include "chat.h"

#include <QDebug>

Chat::Chat(QObject *parent) :
    QObject(parent)
{

}

ChatChannel * Chat::getChannel(QString id)
{
    if (_channels.keys().contains(id))
        return _channels[id];
    else
        return 0;
}

void Chat::updateChannels(QMap<QString, ChatChannel *> channels)
{
    qDebug() << "CHAT :: channels receved";
    _channels = channels;
    emit channelsUpdated();

}

void Chat::updateUsers(QString channelId,
                       QMap<QString, ChatUser *> users)
{
    qDebug() << "CHAT :: users receved";
    getChannel(channelId)->updateUsers(users);
    emit usersUpdated(channelId);
}

void Chat::insertUser(QString channelId, ChatUser *user)
{
    emit userConnected(user);
    getChannel(channelId)->insertUser(user);
}

void Chat::removeUser(QString channelId, QString userId)
{
    emit userDisconnected(new ChatUser(getChannel(channelId)
                          ->getUser(userId)));
    getChannel(channelId)->removeUser(userId);
}
