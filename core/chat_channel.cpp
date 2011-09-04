#include "chat_channel.h"
#include "chat_user.h"
#include "connector.h"

ChatChannel::ChatChannel(Connector *conn, QString id, QString name)
  : QObject(conn),
    _conn(conn), _id (id), _name(name)
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

