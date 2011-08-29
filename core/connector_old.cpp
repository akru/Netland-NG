#include "connector_old.h"
#include "encode.h"

#include <QHostInfo>
#include <QStringList>

#include <QDebug>

const char * ConnectorOld::version = "v8.6";

ConnectorOld::ConnectorOld(QObject *parent)
    : Connector(parent),
      codec(QTextCodec::codecForName("cp-1251")), // All messages from this server encoded in WINDOWS-CP1251
      lastTimeId(0)
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
    recvBuffer += readAll();
    if (recvBuffer.split('\n').count() == 1)    // Verify for not full message
        return;
    QList<QByteArray> recvStrings = recvBuffer.split('\n'); // Splitting messages
    recvBuffer = recvStrings.last();                        // Save not full on buffer
    recvStrings.pop_back();                                 // Remove not full from list
    QList<QByteArray>::const_iterator it;
    for (it = recvStrings.constBegin();
         it != recvStrings.constEnd(); ++it)
    {
        emit readStringReady(*it);
    }
}

void ConnectorOld::sendString(QString str)
{
    write(str.toAscii());
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
        emit authRequestRecv(message.at(0));
        qDebug() << "Auth_r :: " << message;
        break;
    case 'b':       // Information
        emit infoMessage(message.at(0));
        qDebug() << "Inform :: " << message;
        break;
    case 'd':       // Board
        cmd = message.at(0);
        if (cmd == "channels")  // Receved channel list
        {
            message.pop_front();    // Remove command from message
            emit boardChannelsRecv(parseBoardChannels(message.join("\t")));
        }
        else
        {
            if (cmd == "new")
            {
                emit boardNewMessages();
            }
            else
            {
                if (cmd == "skins")
                {
                }
                else
                    emit boardMessagesRecv(parseBoardMessages(message.join("\t")));
            }
        }
        //qDebug() << "DBoard :: " << message;
        break;
    case 'c':       // Chat
        qDebug() << "Chat   :: " << message;
        break;
     default:       // Uncategorsed
        qDebug() << "Unctg  :: " << recvStr;
    }
}

QMap<int, BoardChannel *> ConnectorOld::parseBoardChannels(QString recvMessage)
{
    QMap<int, BoardChannel *> channels;
    /*
      Thanks Assaron

CHANNEL_PROPERTIES = (("id", int_decoder),
                      ("name", unicode_decoder),
                      ("description", unicode_decoder))
      */
    QStringList channelsList = recvMessage.split('\r');
    channelsList.pop_back(); // Remove empty string
    for (QStringList::const_iterator it = channelsList.constBegin();
         it != channelsList.constEnd(); ++it)
    {
        QStringList channel = (*it).split('\t');
        BoardChannel *ch = new BoardChannel(
                    channel.at(0).toInt(),
                    channel.at(1).mid(1),
                    channel.at(2));
        channels.insert(ch->id(), ch);
    }
    return channels;
}

QMap<int, BoardMessage *> ConnectorOld::parseBoardMessages(QString recvMessage)
{
    QMap<int, BoardMessage *> messages;
    /*
      Thanks Assaron

MESSAGE_PROPERTIES = (
    ("id", int_decoder), ("unknown1", int_decoder), ("parent_id", int_decoder),
    ("delete_", int_decoder), ("IP", str_decoder), ("hostname", unicode_decoder),
    ("nick", unicode_decoder), ("body", unicode_decoder),
    ("edit_time", time_decoder), ("channel_id", int_decoder),
    ("unknown2", int_decoder), ("mac", str_decoder), ("zero1", int_decoder),
    ("zero2", int_decoder), ("zero3", int_decoder), ("zero4", int_decoder),
    ("time_id", int_decoder), ("deleted", int_decoder), ("post_time", time_decoder)
    )
      */

    QStringList messagesList = recvMessage.split('\r');
    messagesList.pop_front(); // Remove unknown args
    messagesList.pop_back();  // Remove empty string

    for (QStringList::const_iterator it = messagesList.constBegin();
         it != messagesList.constEnd(); ++it)
    {
        QStringList message = (*it).split('\t');
        QString body = message.at(7);
        body.replace(0x01, '\n');
        bool ok;
        BoardMessage *msg = new BoardMessage(
                    message.at(0).toInt(&ok),
                    message.at(2).toInt(&ok),
                    message.at(9).toInt(&ok),
                    message.at(16).toInt(&ok),
                    message.at(3).toInt(&ok),
                    message.at(17).toInt(&ok),
                    message.at(4),
                    message.at(11),
                    message.at(5),
                    message.at(6),
                    body,
                    message.at(18),
                    message.at(8));
        if (!ok)
        {
            qDebug() << "STRING_PARSER: fault convert!";
            exit(1);
        }
        messages.insert(msg->id(), msg);
        if (msg->timeId() > lastTimeId)
            lastTimeId = msg->timeId();
    }
    return messages;
}

void ConnectorOld::authStringProcessing(QString req)
{
    Encode enc(req, hash);
    QString authCode = enc.getReply();
    QString authString = QString(version) + "\t" +
            authCode + "\tCPU_INFO\tOS_INFO\t" +
            QHostInfo::localHostName() + "\n";
    emit authStringReady(authString);
}

//void ConnectorOld::baseInit()
//{
//    QString nickSetString = QString("cnik\t") + nick + "\n";
//    QString userListString = QString("cUserList\n");

//    QString baseInitString = nickSetString + userListString;
//    emit baseInitReady(baseInitString);
//}

void ConnectorOld::boardUpdateMessages()
{
    qDebug() << "CONNECTOR :: Do update messages";

    QString req = "Dlast\t" +
            QString::number(lastTimeId) + "\t\n";
    sendString(req);
}
