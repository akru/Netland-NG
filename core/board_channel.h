#ifndef BOARD_CHANNEL_H
#define BOARD_CHANNEL_H

#include <QObject>

class BoardChannel : public QObject
{
    Q_OBJECT
public:
    explicit BoardChannel(int id, QString name, QString description);
    inline int id()
    {
        return _id;
    }
    inline QString name()
    {
        return _name;
    }
    inline QString description()
    {
        return _description;
    }

private:
    int _id;
    QString _name, _description;

};

#endif // BOARD_CHANNEL_H
