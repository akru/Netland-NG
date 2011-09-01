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
        if (message.at(0).split(" ").count() == 7)
        {
            if (message.at(0).split(" ").at(2).at(8) == ',')
            {
                qDebug() << "CONN :: Auth success";
                emit authSuccess();
            }
        }
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
            if (cmd == "new")  // New messages
            {
                qDebug() << "CONN :: new messages avaible";
                emit boardNewMessages();
            }
            else
            {
                if (cmd == "skins")
                {
                }
                else
                {
                    if (cmd == "admin")
                    {
                    }
                    else
                    {
                        emit boardMessagesRecv(
                                    parseBoardMessages(message.join("\t")));
                    }
                }
            }
        }
        //qDebug() << "DBoard :: " << message;
        break;
    case 'c':       // Chat
        cmd = message.at(0);
        if (cmd == "main")
        {

        }
        else
        {
            if (cmd == "enter")
            {

            }
            else
            {
                if (cmd == "leave")
                {

                }
                else
                {
                    if (cmd == "private")
                    {

                    }
                    else
                    {
                        if (cmd == "infoGet")
                        {

                        }
                        else
                        {
                            if (cmd == "channels")
                            {

                            }
                            else
                            {
                                if (cmd == "userlist")
                                {

                                }
                            }
                        }
                    }
                }
            }
        }
        qDebug() << "Chat   :: " << message;
        break;
     default:       // Uncategorised
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
        QDateTime millenium(QDate(1999,12,31), QTime(23,00));  // Really?
        BoardMessage *msg = new BoardMessage(
                    message.at(0).toInt(),
                    message.at(2).toInt(),
                    message.at(9).toInt(),
                    message.at(16).toInt(),
                    message.at(17).toInt(),
                    message.at(4),
                    message.at(11),
                    message.at(5),
                    message.at(6),
                    body,
                    millenium.addSecs(message.at(3).toInt()),
                    millenium.addSecs(message.at(18).toInt()),
                    millenium.addSecs(message.at(8).toInt()));
        messages.insert(msg->id(), msg);
        if (msg->timeId() > lastTimeId)
            lastTimeId = msg->timeId();
    }
    return messages;
}


void ConnectorOld::authStringProcessing(QString req)
{
    Encode enc(req, hash);
    QByteArray authCode = enc.getReply().toAscii();
    QByteArray authString = QByteArray(version) + "\t" +
            authCode + "\tCPU_INFO\tOS_INFO\t" +
            QHostInfo::localHostName().toAscii() + "\n";
    write(authString);
}

void ConnectorOld::boardUpdateMessages()
{
    QByteArray req = "Dlast\t" +
            QString::number(lastTimeId).toAscii() + "\t\n";
    write(req);
}

void ConnectorOld::boardAddMessage(BoardChannel *channel,
                                   QString text, int actualityDays)
{
    QByteArray req = "Dadd\t" + QString::number(channel->id()).toAscii() +
            "\t" + QString::number(actualityDaysConvert(actualityDays)).toAscii() +
            "\t" + codec->fromUnicode(_nick) +
            "\t" + codec->fromUnicode(text.replace('\n', '\r')) + "\n";
    write(req);
}

void ConnectorOld::boardAddReply(BoardMessage *message, QString text)
{
    QByteArray req = "Dreply\t" + QString::number(message->id()).toAscii() +
            "\t" + codec->fromUnicode(_nick) + "\t" +
            codec->fromUnicode(text.replace('\n', '\r')) + "\t\t\n";
    write(req);
}

void ConnectorOld::boardEditMessage(BoardMessage *message,
                                    QString text, int actualityDays)
{
    QByteArray req = "Dedit\t" + QString::number(message->id()).toAscii() +
            "\t" + QString::number(message->channelId()).toAscii() +
            "\t" + QString::number(actualityDaysConvert(actualityDays)).toAscii() +
            "\t" + QString::number(message->parentId()).toAscii() +
            "\t" + codec->fromUnicode(_nick) +
            "\t" + codec->fromUnicode(text.replace('\n', '\r')) + "\t\n";
    write(req);
}

void ConnectorOld::boardDeleteMessage(BoardMessage *message)
{
    QByteArray req = "Ddel\t" + QString::number(message->id()).toAscii() +
            "\t" + codec->fromUnicode(_nick) + "\t\t\n";
    write(req);
}

void ConnectorOld::boardUpMessage(BoardMessage *message)
{
    QByteArray req = "Dup\t" + QString::number(message->id()).toAscii() + "\n";
    write(req);
}

int ConnectorOld::actualityDaysConvert(int actualityDays)
{
    QDate today = QDate::currentDate();
    return QDate(1899, 12, 30).daysTo(today.addDays(actualityDays));
}

void ConnectorOld::chatSetNick()
{
    QByteArray nickSetStr = "cnik\t" + _nick.toAscii() + "\n";
    write(nickSetStr);
}

void ConnectorOld::chatUpdateUsers()
{
    write("cUserList\n");
}
