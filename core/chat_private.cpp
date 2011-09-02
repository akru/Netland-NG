#include "chat_private.h"

ChatPrivate::ChatPrivate(QString senderId, QString recieverId, QString text)
    : _sender_id(senderId), _reciever_id(recieverId),
      _text(text), _created(QDateTime::currentDateTime())
{
}
