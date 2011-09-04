#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QMap>

#include <boost/shared_ptr.hpp>
using namespace boost;

class BoardChannel;
class BoardMessage;
class Connector;

class Board : public QObject
{
    Q_OBJECT
public:
    explicit Board(Connector *conn);
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
    Connector *parent;
    QMap<int, shared_ptr<BoardChannel> > _channels;
    QMap<int, shared_ptr<BoardMessage> > _messages;
};

#endif // BOARD_H
