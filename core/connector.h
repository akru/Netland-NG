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
    inline Board * getBoardInstance()
    {
        return _board;
    }
    inline Chat * getChatInstance()
    {
        return _chat;
    }

public slots:
    void setNick(QString nick);
    void connectToServer(QString address, QString port);

signals:
    void authSuccess();
    void infoMessage(QString msg);
    void boardChannelsRecv(QMap<int, BoardChannel *> channels);
    void boardMessagesRecv(QMap<int, BoardMessage *> messages);
    void boardNewMessages();
    void nickIsSet();
    void chatUserEnter(ChatUser *user);
    void chatUserLeave(ChatUser *user);

protected:
    QString _nick;

private:
    void connectAll();

private slots:
    virtual void boardUpdateMessages() = 0;
    virtual void boardAddMessage(BoardChannel *channel,
                                 QString text, int actualityDays) = 0;
    virtual void boardAddReply(BoardMessage *message, QString text) = 0;
    virtual void boardEditMessage(BoardMessage *message,
                                  QString text, int actualityDays) = 0;
    virtual void boardDeleteMessage(BoardMessage *message) = 0;
    virtual void boardUpMessage(BoardMessage *message) = 0;
    virtual void chatSetNick() = 0;
    virtual void chatUpdateUsers() = 0;

private:
    Board *_board;
    Chat *_chat;
};

#endif // CONNECTOR_H
