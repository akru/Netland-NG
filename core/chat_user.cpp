#include "chat_user.h"
#include "connector.h"
#include "chat_private.h"

ChatUser::ChatUser(Connector *conn,
                   QString channelId, QString id, QString nick,
                   QString ip, QString computerName)
    : QObject(conn),
      _conn(conn), _channel_id(channelId), _id(id), _nick(nick),
      _ip(ip), _computer_name(computerName)
{
  connectAll();
}

void ChatUser::connectAll()
{
  connect(_conn, SIGNAL(chatPrivateMessage(shared_ptr<ChatPrivate>)),
          this, SLOT(recvPrivate(shared_ptr<ChatPrivate>)));
}

void ChatUser::recvPrivate(shared_ptr<ChatPrivate> msg)
{
  if (msg->senderId() == _id)
  {
    _history.insert(msg->created(), msg);
    emit newPrivate(msg->text());
  }
}

//void ChatUser::sendPrivate(QString text)
//{
//}
