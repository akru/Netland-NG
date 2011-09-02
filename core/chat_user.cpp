#include "chat_user.h"

ChatUser::ChatUser(QString channelId, QString id,
                   QString nick, QString ip, QString computerName)
    : _channel_id(channelId), _id(id), _nick(nick),
      _ip(ip), _computer_name(computerName)
{
}

ChatUser::ChatUser(ChatUser *user)
    : _channel_id(user->channelId()), _id(user->id()), _nick(user->nick()),
      _ip(user->ip()), _computer_name(user->computerName())
{
}
