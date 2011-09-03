#include "chat_user.h"

ChatUser::ChatUser(QString channelId, QString id,
                   QString nick, QString ip, QString computerName)
    : _channel_id(channelId), _id(id), _nick(nick),
      _ip(ip), _computer_name(computerName)
{
}
