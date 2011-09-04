#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QMap>

#include <boost/shared_ptr.hpp>
using namespace boost;

#include "board_channel.h"
#include "board_message.h"

class Board : public QObject
{
    Q_OBJECT
public:
    explicit Board(QObject *parent = 0);
    inline QList<shared_ptr<BoardChannel> > channels()
    {
        return _channels.values();
    }
    inline shared_ptr<BoardChannel> getChannel(int id)
    {
        return _channels[id];
    }
    inline shared_ptr<BoardMessage> getMessage(int id)
    {
        return _messages[id];
    }

signals:
    void channelsUpdated();
    void messagesUpdated();

    // === Forwarding Board slots === //
    void doAddMessage(int channelId,
                      QString text, int actualityDays);
    void doAddReply(int messageId, QString text);
    void doEditMessage(int messageId,
                       QString text, int actualityDays);
    void doDeleteMessage(int messageId);
    void doUpMessage(int messageId);
    // === ********************** === //

public slots:
    void addMessage(int channelId,
                    QString text, int actualityDays = 30);
    void addReply(int messageId, QString text);
    void editMessage(int messageId,
                     QString text, int actualityDays = 30);
    void deleteMessage(int messageId);
    void upMessage(int messageId);

private slots:
    void updateChannels(QMap<int, shared_ptr<BoardChannel> > channels);
    void updateMessages(QMap<int, shared_ptr<BoardMessage> > messages);

private:
    void rebuildMessagesTree();

private:
    QMap<int, shared_ptr<BoardChannel> > _channels;
    QMap<int, shared_ptr<BoardMessage> > _messages;
};

#endif // BOARD_H
