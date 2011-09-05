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

#ifndef ENCODE_H
#define ENCODE_H

#include <QString>
#include <QByteArray>

typedef unsigned char byte;

class Encode
{
public:
  Encode(QString req, int hash);
  QString getReply();

private:
  size_t sz;
  QByteArray reply;

  int d0c(int x);
  void d84(byte * data, size_t sz, int x);
  void bf8(byte * data, size_t sz);

};

#endif // ENCODE_H
