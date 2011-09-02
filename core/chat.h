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
    inline QList<ChatChannel *> channels()
    {
        return _channels.values();
    }
    ChatChannel * getChannel(QString id);

signals:
    void channelsUpdated();
    void usersUpdated(QString channelId);
    void userConnected(ChatUser *user);
    void userDisconnected(ChatUser *user);

public slots:

private:
    void connectChannels();

private slots:
    void updateChannels(QMap<QString, ChatChannel *> channels);
    void updateUsers(QString channelId,
                     QMap<QString, ChatUser *> users);
    void insertUser(QString channelId, ChatUser *user);
    void removeUser(QString channelId, QString userId);

private:
    QMap<QString, ChatChannel *> _channels;

};

#endif // CHAT_H
