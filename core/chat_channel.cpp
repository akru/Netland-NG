#include "chat_channel.h"

ChatChannel::ChatChannel(QString id, QString name)
    : _id (id), _name(name)
{
}

void ChatChannel::updateUsers(QMap<QString, ChatUser *> users)
{
    _users = users;
}

void ChatChannel::insertUser(ChatUser *user)
{
    _users.insert(user->id(), user);
}

void ChatChannel::removeUser(QString userId)
{
    _users.remove(userId);
}

