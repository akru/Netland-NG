#ifndef CONNECTOR_OLD_H
#define CONNECTOR_OLD_H

#include "connector.h"

#include <QTextCodec>

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

private:
    void connectAll();
    QMap<int, BoardChannel *> parseBoardChannels(QString recvMessage);
    QMap<int, BoardMessage *> parseBoardMessages(QString recvMessage);
    QMap<QString, ChatChannel *>  parseChatChannels(QString recvMessage);
    QMap<QString, ChatUser *> parseChatUsers(QStringList recvMessage);
    ChatUser * parseEnteredUser(QString recvMessage);
    ChatPrivate * parseChatPrivate(QString recvMessage);

    int actualityDaysConvert(int actualityDays);


private slots:
    void readString();
    void stringParser(QByteArray recv);
    void authStringProcessing(QString req);

    void boardUpdateMessages();
    void boardAddMessage(BoardChannel *channel,
                         QString text, int actualityDays);
    void boardAddReply(BoardMessage *message, QString text);
    void boardEditMessage(BoardMessage *message,
                          QString text, int actualityDays);
    void boardDeleteMessage(BoardMessage *message);
    void boardUpMessage(BoardMessage *message);

    void chatSetNick();
    void chatUpdateUsers();

private:
    QTextCodec *codec;
    int lastTimeId;
    QByteArray recvBuffer;

    static const char* version;
    static const int hash = 0x02d45b64;
};

#endif // CONNECTOR_OLD_H
