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

private slots:
    void readString();
    void stringParser(QByteArray recv);
    void authStringProcessing(QString req);
    void sendString(QString str);
    void isConnected();

    void boardUpdateMessages();


private:
    QTextCodec *codec;
    int lastTimeId;
    QByteArray recvBuffer;

    static const char* version;
    static const int hash = 0x02d45b64;
};

#endif // CONNECTOR_OLD_H
