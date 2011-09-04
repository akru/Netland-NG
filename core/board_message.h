#ifndef BOARD_MESSAGE_H
#define BOARD_MESSAGE_H

#include <QObject>
#include <QDateTime>

class Connector;

class BoardMessage : public QObject
{
    Q_OBJECT
public:
    explicit BoardMessage(Connector *conn,
                          int id, int parent_id, int channel_id, int time_id,
                          int deleted, QString ip, QString mac, QString hostname,
                          QString nick, QString body, QDateTime valid_time,
                          QDateTime post_time, QDateTime edit_time);
    inline int id()
    {
        return _id;
    }
    inline int parentId()
    {
        return _parent_id;
    }
    inline int channelId()
    {
        return _channel_id;
    }
    inline int timeId()
    {
        return _time_id;
    }
    inline int deleted()
    {
        return _deleted;
    }
    inline QString ip()
    {
        return _ip;
    }
    inline QString mac()
    {
        return _mac;
    }
    inline QString hostname()
    {
        return _hostname;
    }
    inline QString nick()
    {
        return _nick;
    }
    inline QString body()
    {
        return _body;
    }
    inline QDateTime validTime()
    {
        return _valid_time;
    }
    inline QDateTime postTime()
    {
        return _post_time;
    }
    inline QDateTime editTime()
    {
        return _edit_time;
    }

public slots:
    void addReply(QString text);
    void editMessage(QString text, int actualityDays = 30);
    void deleteMessage();
    void upMessage();

private:
    Connector *_conn;
    int _id, _parent_id, _channel_id, _time_id, _deleted;
    QString _ip, _mac, _hostname, _nick, _body;
    QDateTime _valid_time, _post_time, _edit_time;
};

#endif // BOARD_MESSAGE_H
