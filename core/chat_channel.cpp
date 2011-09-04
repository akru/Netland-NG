#include "chat_channel.h"

ChatChannel::ChatChannel(QString id, QString name)
    : _id (id), _name(name)
{
}

void ChatChannel::updateUsers(QMap<QString, shared_ptr<ChatUser> > users)
{
    _users = users;
}

void ChatChannel::insertUser(shared_ptr<ChatUser> user)
{
    _users.insert(user->id(), user);
}

void ChatChannel::removeUser(QString userId)
{
    _users.remove(userId);
}

