#ifndef MAIN_APPLICATION_H
#define MAIN_APPLICATION_H

#include <QtCore/QCoreApplication>
#include <QHostAddress>

#include "connector.h"

class MainApplication : public QCoreApplication
{
    Q_OBJECT
public:
    MainApplication(int argc, char *argv[]);
    ~MainApplication();

private:
    void connectAll();

public slots:
    void testViewChannels();
    void testViewMessages();

private:
    Connector *conn;
    QHostAddress server;
};

#endif // MAIN_APPLICATION_H
