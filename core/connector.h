#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "board.h"
#include "chat.h"
#include "chat_private.h"

#include <QDateTime>
#include <QTcpSocket>
#include <QMap>

#include <boost/shared_ptr.hpp>
using namespace boost;

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
    void boardChannelsRecv(QMap<int, shared_ptr<BoardChannel>> channels);
    void boardMessagesRecv(QMap<int, shared_ptr<BoardMessage>> messages);
    void boardNewMessages();
    void nickIsSet();
    void chatChannelsRecv(QMap<QString, shared_ptr<ChatChannel>> channels);
    void chatUsersRecv(QString channelId, QMap<QString, shared_ptr<ChatUser>> users);
    void chatUserEnter(QString channelId, shared_ptr<ChatUser> user);
    void chatUserLeave(QString channelId, QString userId);
    void chatPrivateMessage(ChatPrivate *message);

protected:
    QString _nick;

private:
    void connectAll();

private slots:
    virtual void boardUpdateMessages() = 0;
    virtual void boardAddMessage(int channelId,
                                 QString text, int actualityDays) = 0;
    virtual void boardAddReply(int messageId, QString text) = 0;
    virtual void boardEditMessage(int messageId,
                                  QString text, int actualityDays) = 0;
    virtual void boardDeleteMessage(int messageId) = 0;
    virtual void boardUpMessage(int messageId) = 0;
    virtual void chatSetNick() = 0;
    virtual void chatUpdateUsers() = 0;

private:
    Board *_board;
    Chat *_chat;
};

#endif // CONNECTOR_H
