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

#ifndef BOARD_CHANNEL_H
#define BOARD_CHANNEL_H

#include <QObject>

class Connector;

class BoardChannel
    : public QObject
{
  Q_OBJECT
public:
  explicit BoardChannel(Connector *conn,
                        int id, QString name, QString description);
  inline int id()
  {
    return _id;
  }
  inline QString name()
  {
    return _name;
  }
  inline QString description()
  {
    return _description;
  }

signals:
  void addMessageReady(int channelId, QString text, int actualityDays);

public slots:
  void addMessage(QString text, int actualityDays = 30);

private:
  Connector *_conn;
  int _id;
  QString _name, _description;
};

#endif // BOARD_CHANNEL_H
