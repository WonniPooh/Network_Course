#include "tcpthreadpart.h"
#include "tcpclient.h"
#include <assert.h>

TcpThreadPart::TcpThreadPart(TcpClient *main_class, QObject *parent) : QObject(parent)
{
    assert(main_class);
    m_parent = main_class;
    m_port = 0;
    m_prog_id = 0;
    m_tcpsocket = nullptr;
}

TcpThreadPart::~TcpThreadPart()
{
    if(m_tcpsocket)
    {
        if(m_tcpsocket->state() != QAbstractSocket::UnconnectedState)
            slotDisconnect();
        delete m_tcpsocket;
    }
}

void TcpThreadPart::setTcpConfig(QString &server_host, int server_port, int prog_id)
{
    m_host = server_host;
    m_port = server_port;
    m_prog_id = prog_id;
}

void TcpThreadPart::slotTcpConnect()
{
    m_tcpsocket = new QTcpSocket(this);

    connect(m_tcpsocket, &QTcpSocket::connected, m_parent, &TcpClient::slotConnected);
    connect(m_tcpsocket, &QTcpSocket::disconnected, m_parent, &TcpClient::slotDisconnected);
    connect(m_tcpsocket, &QTcpSocket::readyRead, this, &TcpThreadPart::slotMsgRecieved);
    connect(m_parent, &TcpClient::sendMessage, this, &TcpThreadPart::slotSendMessage);
    connect(m_tcpsocket, SIGNAL(error(QAbstractSocket::SocketError)), m_parent, SLOT(slotError(QAbstractSocket::SocketError)));

    m_tcpsocket->connectToHost(m_host, m_port);
    slotSendMessage(QString::number(m_prog_id));
}

void TcpThreadPart::slotSendMessage(QString data_to_send)
{
    QByteArray byte_arr;
    QDataStream out (&byte_arr, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_5);
    out << quint16(0) << data_to_send;

    out.device()->seek(0);
    out << quint16(byte_arr.size() - sizeof(quint16));
    m_tcpsocket -> write(byte_arr);//error handler?
}

void TcpThreadPart::slotMsgRecieved()
{
    QDataStream in(m_tcpsocket);
    in.setVersion(QDataStream::Qt_5_5);
    QString new_msg;
    quint16 sender_id = 0;
    quint16 next_block_size = 0;

    for(;;)
    {
        if(!next_block_size)
        {
            if(m_tcpsocket->bytesAvailable() < sizeof(quint16))
            break;

            in >> next_block_size;
        }

        if(m_tcpsocket->bytesAvailable() < next_block_size)
            break;

        in >> sender_id >> new_msg;

        next_block_size = 0;

        emit messageRecived(sender_id, new_msg);
    }
}

void TcpThreadPart::slotDisconnect()
{
    if(m_tcpsocket->state() != QAbstractSocket::UnconnectedState)
    {
        m_tcpsocket->disconnectFromHost();
        m_tcpsocket->waitForDisconnected(10000);
    }
}
