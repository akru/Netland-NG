#ifndef CHAT_USER_H
#define CHAT_USER_H

#include <QObject>

class Connector;

class ChatUser : public QObject
{
  Q_OBJECT
public:
    explicit ChatUser(Connector *conn,
                      QString channelId, QString id, QString nick,
                      QString ip, QString computerName);
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
    Connector *_conn;
    QString _channel_id, _id, _nick, _ip, _computer_name;

};

#endif // CHAT_USER_H
