/*
 *      This file is part of the Netland-NG.
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 *
 */

#ifndef CHAT_PRIVATE_H
#define CHAT_PRIVATE_H

#include <QObject>
#include <QDateTime>

class ChatPrivate
{
public:
  explicit ChatPrivate(QString channelId, QString senderId,
                       QString recieverId, QString text);
  inline QString channelId()
  {
    return _channel_id;
  }
  inline QString senderId()
  {
    return _sender_id;
  }
  inline QString receiverId()
  {
    return _receiver_id;
  }
  inline QString text()
  {
    return _text;
  }
  inline QDateTime created()
  {
    return _created;
  }
  inline QDateTime delivered()
  {
    return _delivered;
  }
  inline void setDelivered(QDateTime delivered)
  {
    _delivered = delivered;
  }


private:
  QString _channel_id, _sender_id, _receiver_id, _text;
  QDateTime _created, _delivered;
};

#endif // CHAT_PRIVATE_H
