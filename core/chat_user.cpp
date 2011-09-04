#include "chat_user.h"
#include "connector.h"

ChatUser::ChatUser(Connector *conn,
                   QString channelId, QString id, QString nick,
                   QString ip, QString computerName)
    : QObject(conn),
      _conn(conn), _channel_id(channelId), _id(id), _nick(nick),
      _ip(ip), _computer_name(computerName)
{
}
