#ifndef CHAT_CHANNEL_H
#define CHAT_CHANNEL_H

#include <QObject>
#include <QMap>

#include <boost/shared_ptr.hpp>
using namespace boost;

class Connector;
class ChatUser;

class ChatChannel : public QObject
{
  Q_OBJECT
public:
    explicit ChatChannel(Connector *con,
                         QString id, QString name);
    inline QString id()
    {
        return _id;
    }
    inline QString name()
    {
        return _name;
    }
    inline QList<shared_ptr<ChatUser> > users()
    {
        return _users.values();
    }
    inline shared_ptr<ChatUser> getUser(QString id)
    {
        return _users[id];
    }

signals:
    void usersUpdated();
    void userConnected(shared_ptr<ChatUser> user);
    void userDisconnected(shared_ptr<ChatUser> user);

private:
    void connectAll();

private slots:
    void updateUsers(QString channelId,
                     QMap<QString, shared_ptr<ChatUser> > users);
    void insertUser(QString channelId, shared_ptr<ChatUser> user);
    void removeUser(QString channelId, QString userId);

private:
    Connector *_conn;
    QString _id, _name;
    QMap<QString, shared_ptr<ChatUser> > _users;
};

#endif // CHAT_CHANNEL_H
