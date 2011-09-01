#ifndef CHAT_CHANNEL_H
#define CHAT_CHANNEL_H

#include <QObject>

class ChatChannel : public QObject
{
    Q_OBJECT
public:
    explicit ChatChannel(QString name);

    inline QString name()
    {
        return _name;
    }

signals:

public slots:

private:
    QString _name;

};

#endif // CHAT_CHANNEL_H
