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

#ifndef MAIN_APPLICATION_H
#define MAIN_APPLICATION_H

#include <QtCore/QCoreApplication>

#include <boost/shared_ptr.hpp>
using namespace boost;

class Connector;
class ChatUser;
class ChatPrivate;

class MainApplication
    : public QCoreApplication
{
  Q_OBJECT
public:
  MainApplication(int argc, char *argv[]);
  ~MainApplication();

private:
  void connectAll();

public slots:
  void testAuthentification();

  void testBoardViewChannels();
  void testBoardViewMessages();
  void testChatViewChannels();
  void testChatViewChannelUsers();
  void testChatViewConnected(shared_ptr<ChatUser> user);
  void testChatViewDisconnected(shared_ptr<ChatUser> user);
  void testChatPrivateMessage(shared_ptr<ChatPrivate> msg);

private:
  Connector *conn;
  QString server;
};

#endif // MAIN_APPLICATION_H
