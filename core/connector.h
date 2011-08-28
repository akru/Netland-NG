#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "board.h"
#include "chat.h"

#include <QDateTime>
#include <QTcpSocket>
#include <QMap>

class Connector :
        public QTcpSocket
{
    Q_OBJECT
public:
    Connector(QObject *parent = 0);
    ~Connector();
    inline Board * getBoardInstance()
    {
        return _board;
    }
    inline Chat * getChatInstance()
    {
        return _chat;
    }

signals:
    void infoMessage(QString msg);
    void boardChannelsRecv(QMap<int, BoardChannel *> channels);
    void boardMessagesRecv(QMap<int, BoardMessage *> messages);
    void boardNewMessages();

private:
    void connectAll();

private slots:
    virtual void boardUpdateMessages() = 0;

private:
    Board *_board;
    Chat *_chat;
};

#endif // CONNECTOR_H
