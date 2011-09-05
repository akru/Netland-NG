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

#include "connector_old.h"
#include "encode.h"
#include "board.h"
#include "board_channel.h"
#include "board_message.h"
#include "chat.h"
#include "chat_channel.h"
#include "chat_user.h"
#include "chat_private.h"
#include <QDateTime>
#include <QHostInfo>
#include <QStringList>

#include <QDebug>

const char * ConnectorOld::version = "v8.6";

ConnectorOld::ConnectorOld(QObject *parent)
  : Connector(parent),
    // All messages from this server encoded in WINDOWS-CP1251
    codec(QTextCodec::codecForName("cp-1251")), lastTimeId(0)
{
  connectAll();
}

void ConnectorOld::connectAll()
{
  connect(this, SIGNAL(readyRead()),
          this, SLOT(readString()));
  connect(this, SIGNAL(readStringReady(QByteArray)),
          this, SLOT(stringParser(QByteArray)));

  connect(this, SIGNAL(authRequestRecv(QString)),
          this, SLOT(authStringProcessing(QString)));
}

void ConnectorOld::readString()
{
  recvBuffer += readAll();
  if (recvBuffer.split('\n').count() == 1)    // Verify for not full message
    return;
  QList<QByteArray> recvStrings = recvBuffer.split('\n'); // Splitting messages
  recvBuffer = recvStrings.last();                        // Save not full on buffer
  recvStrings.pop_back();                                 // Remove not full from list
  QList<QByteArray>::const_iterator it;
  for (it = recvStrings.constBegin(); it != recvStrings.constEnd(); ++it)
    emit readStringReady(*it);
}

void ConnectorOld::stringParser(QByteArray recv)
{
  QString recvStr = codec->toUnicode(recv);
  QChar category = recvStr[0];                        // First symbol of string
  QStringList message = recvStr.mid(1).split("\t");   // Remove first symbol & splitting
  QString cmd;
  switch (category.toAscii())
  {
  case 'R':       // Authentification request
    qDebug() << "CONN :: Auth_req:" << message;
    emit authRequestRecv(message.at(0));
    break;
  case 'b':       // Information
    qDebug() << "CONN :: Inform:" << message;
    emit infoMessage(message.at(0));
    // Verify for success auth string
    if (message.at(0).split(" ").count() == 7)
      if (message.at(0).split(" ").at(2).at(8) == ',')
        emit authSuccess();
    break;
  case 'd':       // Board
    cmd = message.at(0);
    if (cmd == "channels")  // Receved channel list
    {
      qDebug() << "CONN :: Board: channels recv";
      message.pop_front();    // Remove command from message
      emit boardChannelsRecv(parseBoardChannels(message.join("\t")));
    }
    else
    {
      if (cmd == "new")  // New messages
      {
        qDebug() << "CONN :: Board: new messages avaible";
        emit boardNewMessages();
      }
      else
      {
        if (cmd == "skins")
        {
          qDebug() << "CONN :: Board: \"skins\" cmd recv";
        }
        else
        {
          if (cmd == "admin")
          {
            qDebug() << "CONN :: Board: \"admin\" cmd recv";
          }
          else
          {
            qDebug() << "CONN :: Board: messages recv";
            emit boardMessagesRecv(parseBoardMessages(message.join("\t")));
          }
        }
      }
    }
//    qDebug() << "DBoard :: " << message;
    break;
  case 'c':       // Chat
    cmd = message.at(0);
    message.pop_front();    // Remove command from message
    if (cmd == "main")
    {
      qDebug() << "CONN :: Chat: \"main\" cmd recv";
    }
    else
    {
      if (cmd == "enter")
      {
        qDebug() << "CONN :: Chat: new user entered" << message;
        QString channelId = message.at(0);
        if (getChatInstance()->getChannel(channelId) != NULL)
          emit chatUserEnter(channelId, parseEnteredUser(message));
      }
      else
      {
        if (cmd == "leave")
        {
          qDebug() << "CONN :: Chat: user leave" << message;
          QString channelId = message.at(0);
          shared_ptr<ChatChannel> ch =
              getChatInstance()->getChannel(channelId);
          if (ch != NULL)
            if (ch->getUser(message.at(2)) != NULL)
              emit chatUserLeave(channelId, message.at(2));
        }
        else
        {
          if (cmd == "private")
          {
            qDebug() << "CONN :: Chat: new private message";
            emit chatPrivateMessage(parseChatPrivate(message));
          }
          else
          {
            if (cmd == "infoGet")
            {
              qDebug() << "CONN :: Chat: \"infoGet\" cmd recv";
            }
            else
            {
              if (cmd == "channels")
              {
                qDebug() << "CONN :: Chat: channels recv";
                emit chatChannelsRecv(parseChatChannels(message.join("\t")));
              }
              else
              {
                if (cmd == "userlist")
                {
                  qDebug() << "CONN :: Chat: userlist recv";
                  emit chatUsersRecv(message.at(0), parseChatUsers(message));
                }
              }
            }
          }
        }
      }
    }
//    qDebug() << "Chat   :: " << message;
    break;
  default:       // Uncategorised
    qDebug() << "Unctg  :: " << recvStr;
  }
}

QMap<int, shared_ptr<BoardChannel> >
ConnectorOld::parseBoardChannels(QString recvMessage)
{
  QMap<int, shared_ptr<BoardChannel> > channels;
//  Thanks Assaron
//
//  CHANNEL_PROPERTIES = (("id", int_decoder),
//                        ("name", unicode_decoder),
//                        ("description", unicode_decoder))
  QStringList channelsList = recvMessage.split('\r');
  channelsList.pop_back(); // Remove empty string
  QStringList::const_iterator it;
  for (it = channelsList.constBegin(); it != channelsList.constEnd(); ++it)
  {
    QStringList channel = (*it).split('\t');
    shared_ptr<BoardChannel> ch(
          new BoardChannel(
            this,
            channel.at(0).toInt(),
            channel.at(1).mid(1),
            channel.at(2)));
    channels.insert(ch->id(), ch);
  }
  return channels;
}

QMap<int, shared_ptr<BoardMessage> >
ConnectorOld::parseBoardMessages(QString recvMessage)
{
  QMap<int, shared_ptr<BoardMessage> > messages;
//  Thanks Assaron
//
//  MESSAGE_PROPERTIES = (
//    ("id", int_decoder), ("unknown1", int_decoder), ("parent_id", int_decoder),
//    ("delete_", int_decoder), ("IP", str_decoder), ("hostname", unicode_decoder),
//    ("nick", unicode_decoder), ("body", unicode_decoder),
//    ("edit_time", time_decoder), ("channel_id", int_decoder),
//    ("unknown2", int_decoder), ("mac", str_decoder), ("zero1", int_decoder),
//    ("zero2", int_decoder), ("zero3", int_decoder), ("zero4", int_decoder),
//    ("time_id", int_decoder), ("deleted", int_decoder), ("post_time", time_decoder))
  QStringList messagesList = recvMessage.split('\r');
  messagesList.pop_front(); // Remove unknown args
  messagesList.pop_back();  // Remove empty string

  QStringList::const_iterator it;
  for (it = messagesList.constBegin(); it != messagesList.constEnd(); ++it)
  {
    QStringList message = (*it).split('\t');
    QString body = message.at(7);
    body.replace(0x01, '\n');
    QDateTime millenium(QDate(1999,12,31), QTime(23,00));  // Really?
    shared_ptr<BoardMessage> msg(
          new BoardMessage(
            this,
            message.at(0).toInt(),
            message.at(2).toInt(),
            message.at(9).toInt(),
            message.at(16).toInt(),
            message.at(17).toInt(),
            message.at(4),
            message.at(11),
            message.at(5),
            message.at(6),
            body,
            millenium.addSecs(message.at(3).toInt()),
            millenium.addSecs(message.at(18).toInt()),
            millenium.addSecs(message.at(8).toInt())));
    messages.insert(msg->id(), msg);
    if (msg->timeId() > lastTimeId)
      lastTimeId = msg->timeId();
  }
  return messages;
}

QMap<QString, shared_ptr<ChatChannel> >
ConnectorOld::parseChatChannels(QString recvMessage)
{
  QMap<QString, shared_ptr<ChatChannel> > channels;
  QStringList channelsList = recvMessage.split("\r");
  channelsList.pop_back(); // Remove empty string
  QStringList::const_iterator it;
  for (it = channelsList.constBegin(); it != channelsList.constEnd(); ++it)
  {
    QStringList channel = (*it).split('\t');
    shared_ptr<ChatChannel> ch(
          new ChatChannel(
            this,
            channel.at(0),
            channel.at(1)));
    channels.insert(ch->id(), ch);
  }
  return channels;
}

QMap<QString, shared_ptr<ChatUser> >
ConnectorOld::parseChatUsers(QStringList recvMessage)
{
  QMap<QString, shared_ptr<ChatUser> > users;
  QString channelId = recvMessage.at(0);
  recvMessage.pop_front(); // Remove channelId
  QStringList usersList = recvMessage.join("\t").split("\r");
  usersList.pop_back();   // Remove empty string

  QStringList::const_iterator it;
  for (it = usersList.constBegin(); it != usersList.constEnd(); ++it)
  {
    QStringList user = (*it).split('\t');
    shared_ptr<ChatUser> us(
          new ChatUser(
            this,
            channelId,
            user.at(0),
            user.at(1),
            user.at(2),
            user.at(3)));
    users.insert(us->id(), us);
  }
  return users;
}

shared_ptr<ChatUser>
ConnectorOld::parseEnteredUser(QStringList recvMessage)
{
  shared_ptr<ChatUser> us(
        new ChatUser(
          this,
          recvMessage.at(0),
          recvMessage.at(2),
          recvMessage.at(3),
          recvMessage.at(4),
          recvMessage.at(5)));
  return us;
}

shared_ptr<ChatPrivate>
ConnectorOld::parseChatPrivate(QStringList recvMessage)
{
  shared_ptr<ChatPrivate> cp(
        new ChatPrivate(
          recvMessage.at(0),
          recvMessage.at(1),
          "",
          recvMessage.at(5)));
  return cp;
}

void ConnectorOld::authStringProcessing(QString req)
{
  Encode enc(req, hash);
  QByteArray authCode = enc.getReply().toAscii();
  QByteArray authString = QByteArray(version) + "\t" +
      authCode + "\tCPU_INFO\tOS_INFO\t" +
      QHostInfo::localHostName().toAscii() + "\n";
  write(authString);
}

void ConnectorOld::boardUpdateMessages()
{
  QByteArray req = "Dlast\t" +
      QString::number(lastTimeId).toAscii() + "\t\n";
  write(req);
}

void ConnectorOld::boardAddMessage(int channelId, QString text, int actualityDays)
{
  QByteArray req = "Dadd\t" + QString::number(channelId).toAscii() +
      "\t" + QString::number(actualityDaysConvert(actualityDays)).toAscii() +
      "\t" + codec->fromUnicode(_nick) +
      "\t" + codec->fromUnicode(text.replace('\n', '\r')) + "\n";
  write(req);
}

void ConnectorOld::boardAddReply(int messageId, QString text)
{
  QByteArray req = "Dreply\t" + QString::number(messageId).toAscii() +
      "\t" + codec->fromUnicode(_nick) + "\t" +
      codec->fromUnicode(text.replace('\n', '\r')) + "\t\t\n";
  write(req);
}

void ConnectorOld::boardEditMessage(int messageId, QString text, int actualityDays)
{
  shared_ptr<BoardMessage> msg =
      getBoardInstance()->getMessage(messageId);
  QByteArray req = "Dedit\t" + QString::number(messageId).toAscii() +
      "\t" + QString::number(msg->channelId()).toAscii() +
      "\t" + QString::number(actualityDaysConvert(actualityDays)).toAscii() +
      "\t" + QString::number(msg->parentId()).toAscii() +
      "\t" + codec->fromUnicode(_nick) +
      "\t" + codec->fromUnicode(text.replace('\n', '\r')) + "\t\n";
  write(req);
}

void ConnectorOld::boardDeleteMessage(int messageId)
{
  QByteArray req = "Ddel\t" + QString::number(messageId).toAscii() +
      "\t" + codec->fromUnicode(_nick) + "\t\t\n";
  write(req);
}

void ConnectorOld::boardUpMessage(int messageId)
{
  QByteArray req = "Dup\t" + QString::number(messageId).toAscii() + "\n";
  write(req);
}

int ConnectorOld::actualityDaysConvert(int actualityDays)
{
  QDate today = QDate::currentDate();
  return QDate(1899, 12, 30).daysTo(today.addDays(actualityDays));
}

void ConnectorOld::chatSetNick()
{
  QByteArray nickSetStr = "cnik\t" + codec->fromUnicode(_nick) + "\n";
  write(nickSetStr);
}

void ConnectorOld::chatUpdateUsers()
{
  write("cUserList\n");
}
