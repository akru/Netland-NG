#include "board_message.h"

BoardMessage::BoardMessage(int id, int parent_id, int channel_id, int time_id,
                           int deleted, QString ip, QString mac, QString hostname,
                           QString nick, QString body, QDateTime valid_time,
                           QDateTime post_time, QDateTime edit_time)
    : _id(id), _parent_id(parent_id), _channel_id(channel_id), _time_id(time_id),
      _deleted(deleted),
      _ip(ip), _mac(mac), _hostname(hostname), _nick(nick), _body(body),
      _valid_time(valid_time), _post_time(post_time), _edit_time(edit_time)
{
}
