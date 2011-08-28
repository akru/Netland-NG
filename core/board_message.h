#ifndef BOARD_MESSAGE_H
#define BOARD_MESSAGE_H

#include <QObject>

class BoardMessage : public QObject
{
    Q_OBJECT
public:
    explicit BoardMessage(int id, int parent_id, int channel_id, int time_id, int deleted, int delete_,
                          QString ip, QString mac, QString hostname, QString nick, QString body,
                          QString post_time, QString edit_time);
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
    inline int delete_()
    {
        return _delete_;
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
    inline QString postTime()
    {
        return _post_time;
    }
    inline QString editTime()
    {
        return _edit_time;
    }

signals:

public slots:

private:
    int _id, _parent_id, _channel_id, _time_id, _deleted, _delete_;
    QString _ip, _mac, _hostname, _nick, _body, _post_time, _edit_time;

};

#endif // BOARD_MESSAGE_H
