#ifndef CHAT_PRIVATE_H
#define CHAT_PRIVATE_H

#include <QObject>
#include <QDateTime>


class ChatPrivate
{
public:
    explicit ChatPrivate(QString senderId, QString recieverId, QString text);
    inline QString senderId()
    {
        return _sender_id;
    }
    inline QString recieverId()
    {
        return _reciever_id;
    }
    inline QString text()
    {
        return _text;
    }
    inline QDateTime created()
    {
        return _created;
    }

private:
    QString _sender_id, _reciever_id, _text;
    QDateTime _created;
};

#endif // CHAT_PRIVATE_H
