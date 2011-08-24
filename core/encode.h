#ifndef ENCODE_H
#define ENCODE_H

#include <QString>
#include <QByteArray>

typedef unsigned char byte;

class Encode
{
public:
    Encode(QString req, int hash);
    QString getReply();

private:
    size_t sz;
    QByteArray reply;

    int d0c(int x);
    void d84(byte * data, size_t sz, int x);
    void bf8(byte * data, size_t sz);

};

#endif // ENCODE_H
