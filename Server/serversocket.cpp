#include "serversocket.h"

ServerSocket::ServerSocket(){}

void ServerSocket::init()
{
    m_server = new QTcpServer(this);
    m_port = 5555;

    if (m_server->listen(QHostAddress::Any, m_port))
    {
        m_serverStatus = ServerSocket::Listening;
        connect(m_server, &QTcpServer::newConnection, this, &ServerSocket::onNewConnection);
        slotUpdateStatus_log("<nobr><font color=\"green\">Server started. Listening: " + QString::number(m_port) + "</nobr>");
    }
    else
    {
        slotUpdateStatus_log("<nobr><font color=\"red\">Server is not started. Error code " + m_server->errorString()+ "</nobr>");
    }
}

void ServerSocket::onNewConnection()
{
    if(m_serverStatus == Listening)
    {
        QTcpSocket* m_socket = m_server->nextPendingConnection();
        if (m_socket != nullptr)
        {
            slotUpdateStatus_log("<nobr><font color=\"green\">New client connected</nobr>");
        }
        else
        {
            slotUpdateStatus_log("<nobr><font color=\"red\">New client tried to connect, but it was unsuccessful</nobr>");
        }
    }
}

void ServerSocket::slotUpdateStatus_log(QString status)
{
    emit updateStatus_log(status);
}
