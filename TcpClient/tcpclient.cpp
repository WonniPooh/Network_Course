#include "tcpclient.h"
#include "tcpthreadpart.h"

TcpClient::TcpClient(QObject *parent) : QObject(parent)
{
    close_connection = false;
    connection_settings_set = false;
    connection_status = connection_closed;
    m_prog_id = 0;
    m_port = 0;

    tcp_connection = nullptr;
}

TcpClient::~TcpClient()
{

}

void TcpClient::setUpConnection(std::string host_adr, int server_port, int prog_id)
{
    m_host = QString(host_adr.c_str());
    m_port = server_port;
    m_prog_id = prog_id;
    connection_settings_set = true;

    tcp_connection = new TcpThreadPart(this);
    tcp_connection->setTcpConfig(m_host, m_port, m_prog_id);
    tcp_connection->moveToThread(&tcp_thread);

    connect(&tcp_thread, &QThread::finished, tcp_connection, &QObject::deleteLater);
    connect(&tcp_thread, &QThread::started, tcp_connection, &TcpThreadPart::slotTcpConnect);
    connect(tcp_connection, &TcpThreadPart::messageRecived, this, &TcpClient::slotMsgRecieved);

    tcp_thread.start();
}

void TcpClient::closeConnection()
{
    close_connection = true;
    closeSocketConnection();
}

void TcpClient::slotMsgRecieved(quint16 sender_id, QString msg)
{
    message new_struct;
    new_struct.msg = msg.toStdString();
    new_struct.sender_id = sender_id;
    received_msg_arr.push_back(new_struct);
    emit newMsg();
}

bool TcpClient::connectionStatus()
{
    return connection_status;
}

void TcpClient::slotConnected()
{
    connection_status = connection_established;
}

void TcpClient::slotDisconnected()
{
    connection_status = connection_closed;

    if(!close_connection)
      attemtToReconnect();
}

bool TcpClient::sendMsg(std::string data_to_send)
{
    if(connection_status == connection_established)
    {
        emit sendMessage(QString(data_to_send.c_str()));
        return 1;
    }
    else
        return 0;
}

std::vector<message> TcpClient::getNewMsg()
{
    std::vector<message> ret;
    std::swap(ret, received_msg_arr);
    return ret;
}

void TcpClient::closeSocketConnection()
{
    emit closeTcpConnection();
}

void TcpClient::slotError(QAbstractSocket::SocketError err)
{
   connection_status = connection_closed;

   if(err != QAbstractSocket::HostNotFoundError || QAbstractSocket::SocketResourceError ||
             QAbstractSocket::AddressInUseError || QAbstractSocket::ProxyAuthenticationRequiredError)
   {
       closeSocketConnection();
   }
}


std::string TcpClient::getSocketError()
{
    return tcp_error;
}

void TcpClient::attemtToReconnect()
{
    if(tcp_thread.isRunning())
    {
        emit closeTcpConnection();
        tcp_thread.exit();
    }
    delete tcp_connection;

    setUpConnection(m_host.toStdString(), m_port, m_prog_id);
}
