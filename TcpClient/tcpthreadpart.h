#ifndef TCPTHREADPART_H
#define TCPTHREADPART_H

#include <QtNetwork>
#include <QObject>

class TcpClient;

class TcpThreadPart : public QObject
{
    Q_OBJECT
public:
    explicit TcpThreadPart(TcpClient* main_class, QObject *parent = 0);
    ~TcpThreadPart();
    void setTcpConfig(QString& server_host, int server_port, int prog_id);

signals:
    void messageRecived(quint16 sender_id, QString new_message);

public slots:
    void slotDisconnect();
    void slotTcpConnect();
    void slotSendMessage(QString data_to_send);
    void slotMsgRecieved();

private:
    QString m_host;
    int m_port;
    int m_prog_id;

    QTcpSocket* m_tcpsocket;
    TcpClient* m_parent;

};

#endif // TCPTHREADPART_H
