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
