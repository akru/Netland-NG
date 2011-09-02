#ifndef CHAT_USER_H
#define CHAT_USER_H

#include <QString>

class ChatUser
{
public:
    explicit ChatUser(QString channelId, QString id, QString nick,
                      QString ip, QString computerName);
    ChatUser(ChatUser *user);
    inline QString id()
    {
        return _id;
    }
    inline QString channelId()
    {
        return _channel_id;
    }
    inline QString nick()
    {
        return _nick;
    }
    inline QString ip()
    {
        return _ip;
    }
    inline QString computerName()
    {
        return _computer_name;
    }

private:
    QString _channel_id, _id, _nick, _ip, _computer_name;

};

#endif // CHAT_USER_H
