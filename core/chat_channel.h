#ifndef CHAT_CHANNEL_H
#define CHAT_CHANNEL_H

#include <QObject>
#include <QMap>

#include "chat_user.h"

class ChatChannel : public QObject
{
    Q_OBJECT
public:
    explicit ChatChannel(QString id, QString name);
    inline QString id()
    {
        return _id;
    }
    inline QString name()
    {
        return _name;
    }
    inline QList<ChatUser *> users()
    {
        return _users.values();
    }
    inline ChatUser * getUser(QString id)
    {
        return _users[id];
    }

signals:

public slots:
    void updateUsers(QMap<QString, ChatUser *> users);
    void insertUser(ChatUser *user);
    void removeUser(QString userId);

private:
    QString _id, _name;
    QMap<QString, ChatUser *> _users;
};

#endif // CHAT_CHANNEL_H
