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

#include "chat.h"
#include "chat_channel.h"
#include "chat_user.h"
#include "chat_private.h"
#include "connector.h"

Chat::Chat(Connector *conn)
  : QObject(conn), _conn(conn)
{
  connectAll();
}

void Chat::connectAll()
{
  connect(_conn, SIGNAL(chatChannelsRecv(QMap<QString,shared_ptr<ChatChannel> >)),
          this, SLOT(updateChannels(QMap<QString,shared_ptr<ChatChannel> >)));
}

void Chat::updateChannels(QMap<QString, shared_ptr<ChatChannel> > channels)
{
  _channels = channels;
  emit channelsUpdated();
}
