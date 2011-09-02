#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QMap>

#include "board_channel.h"
#include "board_message.h"

class Board : public QObject
{
    Q_OBJECT
public:
    explicit Board(QObject *parent = 0);
    ~Board();
    inline QList<BoardChannel *> channels()
    {
        return _channels.values();
    }
    BoardChannel * getChannel(int id);
    BoardMessage * getMessage(int id);

signals:
    void channelsUpdated();
    void messagesUpdated();

    void doAddMessage(BoardChannel *channel,
                      QString text, int actualityDays);
    void doAddReply(BoardMessage *message, QString text);
    void doEditMessage(BoardMessage *message,
                       QString text, int actualityDays);
    void doDeleteMessage(BoardMessage *message);
    void doUpMessage(BoardMessage *message);

public slots:
    void addMessage(BoardChannel *channel,
                    QString text, int actualityDays = 30);
    void addReply(BoardMessage *message, QString text);
    void editMessage(BoardMessage *message,
                     QString text, int actualityDays = 30);
    void deleteMessage(BoardMessage *message);
    void upMessage(BoardMessage *message);

private slots:
    void updateChannels(QMap<int, BoardChannel *> channels);
    void updateMessages(QMap<int, BoardMessage *> messages);

private:
    void rebuildMessagesTree();

private:
    QMap<int, BoardChannel *> _channels;
    QMap<int, BoardMessage *> _messages;
};

#endif // BOARD_H
