#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QTcpSocket>

class Connector :
        public QTcpSocket
{
    Q_OBJECT
public:
    Connector(QObject *parent = 0);

signals:
    void infoMessage(QString msg);
    void ipConform();
    void userListRecv();
};

#endif // CONNECTOR_H
