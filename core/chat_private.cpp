#include "chat_private.h"

ChatPrivate::ChatPrivate(QString channelId, QString senderId,
                         QString recieverId, QString text)
    : _channel_id(channelId), _sender_id(senderId), _reciever_id(recieverId),
      _text(text), _created(QDateTime::currentDateTime())
{
}
