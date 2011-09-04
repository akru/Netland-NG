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
        return shared_ptr<ChatUser>(_users[id]);
    }
    void updateUsers(QMap<QString, shared_ptr<ChatUser> > users);
    void insertUser(shared_ptr<ChatUser> user);
    void removeUser(QString userId);

private:
    Connector *_conn;
    QString _id, _name;
    QMap<QString, shared_ptr<ChatUser> > _users;
};

#endif // CHAT_CHANNEL_H
