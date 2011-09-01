#ifndef CHAT_H
#define CHAT_H

#include <QObject>
#include <QMap>

#include "chat_channel.h"
#include "chat_user.h"

class Chat : public QObject
{
    Q_OBJECT
public:
    explicit Chat(QObject *parent = 0);

signals:

public slots:

private:
    QList<ChatChannel *> _channels;
    QMap<int, ChatUser *> _users;

};

#endif // CHAT_H
