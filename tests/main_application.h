#ifndef MAIN_APPLICATION_H
#define MAIN_APPLICATION_H

#include <QtCore/QCoreApplication>

#include <boost/shared_ptr.hpp>
using namespace boost;

class Connector;
class ChatUser;
class ChatPrivate;

class MainApplication : public QCoreApplication
{
    Q_OBJECT
public:
    MainApplication(int argc, char *argv[]);
    ~MainApplication();

private:
    void connectAll();

public slots:
    void testAuthentification();

    void testBoardViewChannels();
    void testBoardViewMessages();
    void testChatViewChannels();
    void testChatViewChannelUsers();
    void testChatViewConnected(shared_ptr<ChatUser> user);
    void testChatViewDisconnected(shared_ptr<ChatUser> user);
    void testChatPrivateMessage(shared_ptr<ChatPrivate> msg);

private:
    Connector *conn;
    QString server;
};

#endif // MAIN_APPLICATION_H
