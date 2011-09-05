#ifndef CHAT_USER_H
#define CHAT_USER_H

#include <QObject>
#include <QMap>
#include <QDateTime>

#include <boost/shared_ptr.hpp>
using namespace boost;

class Connector;
class ChatPrivate;

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
    inline QMap<QDateTime, shared_ptr<ChatPrivate> > history()
    {
        return _history;
    }

signals:
    void newPrivate(QString text);

public slots:
//    void sendPrivate(QString text);

private slots:
    void recvPrivate(shared_ptr<ChatPrivate> msg);

private:
    void connectAll();

private:
    Connector *_conn;
    QString _channel_id, _id, _nick, _ip, _computer_name;
    QMap<QDateTime, shared_ptr<ChatPrivate> > _history;

};

#endif // CHAT_USER_H
