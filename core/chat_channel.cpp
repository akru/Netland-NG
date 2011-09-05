#include "chat_channel.h"
#include "chat_user.h"
#include "connector.h"

ChatChannel::ChatChannel(Connector *conn, QString id, QString name)
  : QObject(conn),
    _conn(conn), _id (id), _name(name)
{
  connectAll();
}

void ChatChannel::connectAll()
{
  connect(_conn, SIGNAL(chatUsersRecv(QString,QMap<QString,shared_ptr<ChatUser> >)),
          this, SLOT(updateUsers(QString,QMap<QString,shared_ptr<ChatUser> >)));
  connect(_conn, SIGNAL(chatUserEnter(QString,shared_ptr<ChatUser>)),
          this, SLOT(insertUser(QString,shared_ptr<ChatUser>)));
  connect(_conn, SIGNAL(chatUserLeave(QString,QString)),
          this, SLOT(removeUser(QString,QString)));
}

void ChatChannel::updateUsers(QString channelId,
                              QMap<QString, shared_ptr<ChatUser> > users)
{
  if (channelId == _id)
  {
    _users = users;
    emit usersUpdated();
  }
}

void ChatChannel::insertUser(QString channelId, shared_ptr<ChatUser> user)
{
  if (channelId == _id)
  {
    _users.insert(user->id(), user);
    emit userConnected(user);
    emit usersUpdated();
  }
}

void ChatChannel::removeUser(QString channelId, QString userId)
{
  if (channelId == _id)
  {
    emit userDisconnected(getUser(userId));
    _users.remove(userId);
    emit usersUpdated();
  }
}

