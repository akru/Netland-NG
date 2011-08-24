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
    void readStringReady(QString str);
    void authStringReady(QString authStr);
    void authRequestRecv(QString req);
    void baseInitReady(QString baseInitStr);

private:
    void connectAll();

private slots:
    void readString();
    void stringParser(QString recvStr);
    void authStringProcessing(QString req);
    void sendString(QString str);
    void isConnected();


private:
    QString hostname;
    QString nick;
    QTextCodec *codec;

    static const char* version;
    static const int hash = 0x02d45b64;
};

#endif // CONNECTOR_OLD_H
