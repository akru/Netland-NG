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
    inline BoardChannel * getChannel(int id)
    {
        return _channels[id];
    }
    inline BoardMessage * getMessage(int id)
    {
        return _messages[id];
    }

signals:
    void channelsUpdated();
    void messagesUpdated();

public slots:
    void updateChannels(QMap<int, BoardChannel *> channels);
    void updateMessages(QMap<int, BoardMessage *> messages);

private:
    void rebuildMessagesTree();

private:
    QMap<int, BoardChannel *> _channels;
    QMap<int, BoardMessage *> _messages;
};

#endif // BOARD_H
