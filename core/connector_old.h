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

#ifndef CONNECTOR_OLD_H
#define CONNECTOR_OLD_H

#include <QTextCodec>
#include "connector.h"

class ConnectorOld :
        public Connector
{
    Q_OBJECT
public:
    ConnectorOld(QObject *parent = 0);

signals:
    void readStringReady(QByteArray recv);
    void authStringReady(QString authStr);
    void authRequestRecv(QString req);

public slots:
    // === Board              === //
    void boardUpdateMessages();
    void boardAddMessage(int channelId,
                         QString text, int actualityDays);
    void boardAddReply(int messageId, QString text);
    void boardEditMessage(int messageId,
                          QString text, int actualityDays);
    void boardDeleteMessage(int messageId);
    void boardUpMessage(int messageId);

    // === Chat               === //
    void chatSetNick();
    void chatUpdateUsers();

private:
    void connectAll();

    QMap<int, shared_ptr<BoardChannel> >
    parseBoardChannels(QString recvMessage);

    QMap<int, shared_ptr<BoardMessage> >
    parseBoardMessages(QString recvMessage);

    QMap<QString, shared_ptr<ChatChannel> >
    parseChatChannels(QString recvMessage);

    QMap<QString, shared_ptr<ChatUser> >
    parseChatUsers(QStringList recvMessage);

    shared_ptr<ChatUser>
    parseEnteredUser(QStringList recvMessage);

    shared_ptr<ChatPrivate>
    parseChatPrivate(QStringList recvMessage);

    int actualityDaysConvert(int actualityDays);

private slots:
    // === Common             === //
    void readString();
    void stringParser(QByteArray recv);
    void authStringProcessing(QString req);

private:
    QTextCodec *codec;
    int lastTimeId;
    QByteArray recvBuffer;

    // TODO: Read this constants from .config file
    static const char* version;
    static const int hash = 0x02d45b64;
};

#endif // CONNECTOR_OLD_H
