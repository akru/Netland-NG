#include "connector.h"

Connector::Connector(QObject *parent) :
        QTcpSocket(parent)
{
}
