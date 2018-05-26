#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QAbstractSocket>
#include <assert.h>
#include <QString>
#include <QThread>
#include <stdlib.h>

class TcpThreadPart;

struct message
{
    uint16_t sender_id;
    std::string msg;
};

enum
{
    connection_closed,
    connection_established
};

class TcpClient:public QObject
{
    Q_OBJECT

public slots:
    void slotConnected();
    void slotMsgRecieved(quint16 sender_id, QString msg);
    void slotDisconnected();
    void slotError(QAbstractSocket::SocketError err);

signals:
    void closeTcpConnection();
    void sendMessage(QString data);
    void newMsg();

private:
    bool                        close_connection;
    bool                        connection_settings_set;
    bool                        connection_status;

    int                         m_prog_id;
    int                         m_port;
    QString                     m_host;

    QThread                     tcp_thread;
    TcpThreadPart*              tcp_connection;

    std::string                 tcp_error;
    std::vector<message>        received_msg_arr;

    void attemtToReconnect();
    void closeSocketConnection();

public:
    explicit TcpClient(QObject *parent = 0);
    ~TcpClient();
    void setUpConnection(std::string host_adr, int server_port, int m_prog_id);
    void closeConnection();
    bool connectionStatus();

    bool sendMsg(std::string data_to_send);
    std::vector<message> getNewMsg();

    std::string getSocketError();
};

#endif // TCPCLIENT_H
