#include "connector_old.h"
#include "encode.h"

#include <QHostInfo>
#include <QStringList>

#include <QDebug>

const char * ConnectorOld::version = "v8.6";

ConnectorOld::ConnectorOld(QObject *parent)
    : Connector(parent),
      hostname(QHostInfo::localHostName()),
      nick("akru"),
      codec(QTextCodec::codecForName("cp-1251"))
{
    connectAll();
}

void ConnectorOld::connectAll()
{
    connect(this, SIGNAL(readyRead()),
            this, SLOT(readString()));
    connect(this, SIGNAL(readStringReady(QString)),
            this, SLOT(stringParser(QString)));
    connect(this, SIGNAL(authRequestRecv(QString)),
            this, SLOT(authStringProcessing(QString)));
    connect(this, SIGNAL(authStringReady(QString)),
            this, SLOT(sendString(QString)));
    connect(this, SIGNAL(connected()),
            this, SLOT(isConnected()));
}

void ConnectorOld::isConnected()
{
    qDebug() << "connected";
}

void ConnectorOld::readString()
{
    emit readStringReady(codec->toUnicode(readAll()));
}

void ConnectorOld::sendString(QString str)
{
    write(str.toAscii());
}

void ConnectorOld::stringParser(QString recvStr)
{
    QString cmd;
    QStringList recvStringList = recvStr.split("\n");
    recvStringList.pop_back();

    QStringList::const_iterator i;
    for (i = recvStringList.constBegin(); i != recvStringList.constEnd(); ++i)
    {
        switch ((*i)[0].toAscii())
        {
        case 'R':
            emit authRequestRecv((*i).mid(1));
            break;
        case 'b':
            qDebug() << "Inf :: " << (*i).mid(1);
            emit infoMessage((*i).mid(1));
            break;
        case 'c':
            cmd = (*i).split('\t')[0];
            if (cmd == QString("cip"))
                emit ipConform();
            if (cmd == QString("cuserlist"))
                emit userListRecv();
            qDebug() << *i;
            break;
         default:
            qDebug() << *i;
        }
    }

}

void ConnectorOld::authStringProcessing(QString req)
{
    Encode enc(req, hash);
    QString authCode = enc.getReply();
    QString authString = QString(version) + "\t" +
                         authCode + "\tCPU_INFO\tOS_INFO\t" + hostname + "\n";
    emit authStringReady(authString);
}

//void ConnectorOld::baseInit()
//{
//    QString nickSetString = QString("cnik\t") + nick + "\n";
//    QString userListString = QString("cUserList\n");

//    QString baseInitString = nickSetString + userListString;
//    emit baseInitReady(baseInitString);
//}
