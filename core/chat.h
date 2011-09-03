#ifndef CHAT_H
#define CHAT_H

#include <QObject>
#include <QMap>

#include <boost/shared_ptr.hpp>
using namespace boost;

#include "chat_channel.h"
#include "chat_user.h"

class Chat : public QObject
{
    Q_OBJECT
public:
    explicit Chat(QObject *parent = 0);
    inline QList<shared_ptr<ChatChannel>> channels()
    {
        return _channels.values();
    }
    inline shared_ptr<ChatChannel> getChannel(QString id)
    {
        return _channels[id];
    }

signals:
    void channelsUpdated();
    void usersUpdated(QString channelId);
    void userConnected(shared_ptr<ChatUser> user);
    void userDisconnected(shared_ptr<ChatUser> user);

public slots:

private:
    void connectChannels();

private slots:
    void updateChannels(QMap<QString, shared_ptr<ChatChannel>> channels);
    void updateUsers(QString channelId,
                     QMap<QString, shared_ptr<ChatUser>> users);
    void insertUser(QString channelId, shared_ptr<ChatUser> user);
    void removeUser(QString channelId, QString userId);

private:
    QMap<QString, shared_ptr<ChatChannel>> _channels;

};

#endif // CHAT_H
