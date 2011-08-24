#include <QtCore/QCoreApplication>
#include <QHostAddress>

#include "connector.h"
#include "connector_old.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Connector *conn = new ConnectorOld;
    QHostAddress server;
    server.setAddress("192.168.111.210");
    conn->connectToHost(server, 1539);
    return a.exec();
}

