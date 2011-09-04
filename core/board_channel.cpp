#include "board_channel.h"
#include "connector.h"

BoardChannel::BoardChannel(Connector *conn,
                           int id, QString name, QString description)
  : _conn(conn), _id(id), _name(name), _description(description)
{
}

void BoardChannel::addMessage(QString text, int actualityDays)
{
  _conn->boardAddMessage(_id, text, actualityDays);
}
