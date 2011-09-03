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

    QMap<int, shared_ptr<BoardChannel>>
    parseBoardChannels(QString recvMessage);

    QMap<int, shared_ptr<BoardMessage>>
    parseBoardMessages(QString recvMessage);

    QMap<QString, shared_ptr<ChatChannel>>
    parseChatChannels(QString recvMessage);

    QMap<QString, shared_ptr<ChatUser> >
    parseChatUsers(QStringList recvMessage);

    shared_ptr<ChatUser>
    parseEnteredUser(QString recvMessage);

    ChatPrivate *
    parseChatPrivate(QString recvMessage);

    int actualityDaysConvert(int actualityDays);


private slots:
    void readString();
    void stringParser(QByteArray recv);
    void authStringProcessing(QString req);

    void boardUpdateMessages();
    void boardAddMessage(int channelId,
                         QString text, int actualityDays);
    void boardAddReply(int messageId, QString text);
    void boardEditMessage(int messageId,
                          QString text, int actualityDays);
    void boardDeleteMessage(int messageId);
    void boardUpMessage(int messageId);

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
