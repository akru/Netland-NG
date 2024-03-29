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

/*
  Thanks Assaron, this code really work,
  but it's really fuck your mind too.
 */

#include "encode.h"

Encode::Encode(QString req, int hash)
  : reply(req.toAscii())
{
  sz = reply.size();

  byte * data = (byte *) reply.data();
  bf8(data, sz);
  d84(data, sz, 0xd26fa8c0); // 192.168.111.210
  bf8(data, sz);
  d84(data, sz, 0x55555555);
  bf8(data, sz);
  d84(data, sz, hash);
  bf8(data, sz);
}

QString Encode::getReply()
{
  QString res;
  byte *data = (byte *) reply.data();
  for (size_t i = 0; i < sz; ++i)
  {
      res.append(char('P' - (data[i] >> 4)));
      res.append(char('P' - (data[i] & 0xF)));
  }
  return res;
}

int Encode::d0c(int x)
{
  const int M = 0x7FFFFFFF;
  int res = 0;
  for (int i = 0; i < 4; ++i)
  {
    int y = x & 0xFF;
    x >>= 8;
    int z = y;
    for (int j = 0; j <= i; ++j)
    {
      z *= y;
      z %= M;
    }
    res += z;
    res %= M;
  }
  return res;
}

void Encode::d84(byte * data, size_t sz, int x)
{
  int * p = reinterpret_cast<int *>(data);
  x = d0c(x);
  int n = sz / sizeof(int);
  for (int i = 0; i < n; ++i)
    p[i] ^= x;
}

void Encode::bf8(byte * data, size_t sz)
{
  byte data0 = data[sz-1] ^ data[sz-2] ^ data[0] ^ (sz % 0x100);
  for (size_t i = sz - 1; i > 0; --i)
    data[i] ^= data[i-1];
  data[0] = data0;
  for (size_t i = 0; i < sz; ++i)
    data[i] = (data[i] >> 4) + (data[i] << 4);
}
