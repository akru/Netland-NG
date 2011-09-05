#ifndef CHAT_H
#define CHAT_H

#include <QObject>
#include <QMap>

#include <boost/shared_ptr.hpp>
using namespace boost;

class ChatChannel;
class ChatUser;
class ChatPrivate;

class Chat : public QObject
{
    Q_OBJECT
public:
    explicit Chat(QObject *parent = 0);
    inline QList<shared_ptr<ChatChannel> > channels()
    {
        return _channels.values();
    }
    inline shared_ptr<ChatChannel> getChannel(QString id)
    {
        return _channels[id];
    }

signals:
    void channelsUpdated();
public slots:

private slots:
    void updateChannels(QMap<QString, shared_ptr<ChatChannel> > channels);

private:
    QMap<QString, shared_ptr<ChatChannel> > _channels;
};

#endif // CHAT_H
