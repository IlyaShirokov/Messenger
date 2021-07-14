#include "serversocket.h"

ServerSocket::ServerSocket(QObject *parent) : QObject(parent)
{
    m_serverStatus = NotListening;
}

ServerSocket::~ServerSocket()
{
    onStopingClicked();
}

void ServerSocket::initServer(QString ipAddres, qint16 port)
{
    m_server = new QTcpServer(this);
    m_port = port;

    if(m_serverStatus == NotListening)
    {
        if (m_server->listen(QHostAddress(ipAddres), m_port))
        {
            m_serverStatus = ServerSocket::Listening;
            connect(m_server, &QTcpServer::newConnection, this, &ServerSocket::onNewConnection);
            emit updateStatus_log("<nobr><font color=\"green\">Server started. Listening: " + QString::number(m_port) + "</nobr>");
        }
        else
        {
            emit updateStatus_log("<nobr><font color=\"red\">Server is not started. Error code " + m_server->errorString()+ "</nobr>");
        }
    }
}

void ServerSocket::onNewConnection()
{
    if(m_serverStatus == Listening)
    {
        QTcpSocket* m_socket = m_server->nextPendingConnection();
        if (m_socket != nullptr)
        {
            ActiveClient* activeClient = new ActiveClient(m_socket->socketDescriptor(), m_socket);
            activeClient->setDialogSocket(nullptr);
            ListOfClients.append(activeClient);

            connect(ListOfClients.last()->getSocket(), &QTcpSocket::readyRead, this, &ServerSocket::slotServerRead);
            connect(ListOfClients.last()->getSocket(), &QTcpSocket::disconnected, this, &ServerSocket::slotClientDisconnected);

            ListOfClients.last()->addToHistoryOfMessages("<nobr><font color=\"green\">Сonnected to server<br></nobr>");
            emit updateStatus_log("<nobr><font color=\"green\">New client connected</nobr>");
        }
        else
        {
            emit updateStatus_log("<nobr><font color=\"red\">New client tried to connect, but it was unsuccessful</nobr>");
        }
    }
}

void ServerSocket::slotServerRead()
{
    QTcpSocket* clientSocket = (QTcpSocket*)sender(); // Получаем объект сокета, который вызвал данный слот
    int index_of_client;
    for(index_of_client = 0; index_of_client < ListOfClients.length(); index_of_client++)
    {
        if (ListOfClients[index_of_client]->getSocket() == clientSocket)
            break;
    }

    m_blockSize = 0;
    QDataStream stream_in(clientSocket);
    Message msg_in;

    if(m_blockSize == 0)
    {
        while (clientSocket->bytesAvailable() < (int)sizeof(quint16))
        {
            continue;
        }
        stream_in >> m_blockSize;
    }

    while (clientSocket->bytesAvailable() < m_blockSize)
    {
        continue;
    }
    m_blockSize = 0;

    stream_in >> msg_in;

    switch (msg_in.getTypeMessage())
    {
    case Message::comAutchRequest:
        updateStatus_log("<nobr><font color=\"green\">Got hostname: </nobr>" + msg_in.getSenderName());
        ListOfClients[index_of_client]->setName(msg_in.getSenderName());
        ListNameOfClients.append(msg_in.getSenderName());
        emit sendClientToMainWindow(ListNameOfClients);
        break;
    case Message::comTextMessage:
        if (ListOfClients[index_of_client]->getDialogSocket() != nullptr)
        {
            int dialogWith;
            for(dialogWith = 0; dialogWith < ListOfClients.length(); dialogWith++)
            {
                if (ListOfClients[dialogWith]->getSocket() == ListOfClients[index_of_client]->getDialogSocket())
                    break;
            }

            ListOfClients[index_of_client]->addToHistoryOfMessages(ListOfClients[index_of_client]->getName() + ": " + msg_in.getTextData());
            ListOfClients[dialogWith]->addToHistoryOfMessages(ListOfClients[index_of_client]->getName() + ": " + msg_in.getTextData());
            Message msg_out(msg_in.getTextData(), Message::comTextMessage, ListOfClients[index_of_client]->getName(), ListOfClients[dialogWith]->getName());
            sendMessage(msg_out, ListOfClients[index_of_client]->getDialogSocket());
        }
        break;
    case Message::comStartDialogWithUser:
        if (msg_in.getDestinationName() != "")
        {
            int dialogWith;
            for(dialogWith = 0; dialogWith < ListOfClients.length(); dialogWith++)
            {
                if (ListOfClients[dialogWith]->getName() == msg_in.getDestinationName())
                    break;
            }
            ListOfClients[index_of_client]->setDialogSocket(ListOfClients[dialogWith]->getSocket());
            break;
        }
    }
}

void ServerSocket::slotClientDisconnected()
{
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    int index_of_client;
    for(index_of_client = 0; index_of_client < ListOfClients.length(); index_of_client++)
    {
        if (ListOfClients[index_of_client]->getSocket() == clientSocket)
            break;
    }

    updateStatus_log("<nobr><font color=\"red\">Client \"" + ListOfClients[index_of_client]->getName() + "\" disconnected </nobr>");
    ListOfClients[index_of_client]->getSocket()->close();
    for (int i = 0; i < ListNameOfClients.length(); i++)
        if (ListNameOfClients[i] == ListOfClients[index_of_client]->getName())
            ListNameOfClients.removeAt(i);
    emit sendClientToMainWindow(ListNameOfClients);
    ListOfClients.remove(index_of_client);
}

void ServerSocket::getMessageFromWindow(QString data)
{
    int index_of_client;
    for(index_of_client = 0; index_of_client < ListOfClients.length(); index_of_client++)
    {
        if (ListOfClients[index_of_client]->getSocket() == currentClient)
            break;
    }

    Message msg_out(data, Message::comTextMessage, "Server", ListOfClients[index_of_client]->getName());
    sendMessage(msg_out, ListOfClients[index_of_client]->getDialogSocket());

    ListOfClients[index_of_client]->addToHistoryOfMessages("Server: " + data);

    if (currentClient == ListOfClients[index_of_client]->getSocket())
        updateStatus_messages(ListOfClients[index_of_client]->getHistoryOfMessages());
}

void ServerSocket::sendMessage(Message &msg, QTcpSocket* client)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << msg;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    client->write(block);
}

void ServerSocket::onStopingClicked()
{
    if(m_serverStatus == Listening)
    {
        for(int i = 0; i < ListOfClients.length(); i++)
        {
            Message msg_out("Server closed", Message::comServerClosed, "Server", ListOfClients[i]->getName());
            sendMessage(msg_out, ListOfClients[i]->getSocket());
        }

        sendClientToMainWindow(ListNameOfClients);
        m_server->close();
        m_serverStatus = NotListening;
        updateStatus_log("<nobr><font color=\"red\">Server stopped</nobr>");
    }
}

void ServerSocket::selectCurrentClient(QString arg_name)
{
    int index_of_client;
    for(index_of_client = 0; index_of_client < ListOfClients.length(); index_of_client++)
    {
        if (ListOfClients[index_of_client]->getName() == arg_name)
            break;
    }

    m_nameClientWithCurrentDialog = arg_name;
    updateStatus_messages(ListOfClients[index_of_client]->getHistoryOfMessages());

    currentClient = ListOfClients[index_of_client]->getSocket();
}

void ServerSocket::sendListOfActiveClient(QStringList test)
{
    QString stringofClients = "";
    for (int i = 0; i < test.length(); i++)
    {
        stringofClients += test[i];
        stringofClients += ";";
    }

    for(int i = 0; i < ListOfClients.length(); i++)
    {
        Message msg(stringofClients, Message::comUsersOnline, "Server", ListOfClients[i]->getName());
        sendMessage(msg, ListOfClients[i]->getSocket());
    }
}

void ServerSocket::getMessageFromMainWindow(QString data, bool toAll)
{
    int index_of_client;
    for(index_of_client = 0; index_of_client < ListOfClients.length(); index_of_client++)
    {
        if (ListOfClients[index_of_client]->getName() == m_nameClientWithCurrentDialog)
            break;
    }

    if (toAll)
    {
        for(int i = 0; i < ListOfClients.length(); i++)
        {
            Message msg(data, Message::comTextMessage, "Server", ListOfClients[i]->getName());
            sendMessage(msg, ListOfClients[i]->getSocket());
        }
    }
    else
    {
        Message msg(data, Message::comTextMessage, "Server", ListOfClients[index_of_client]->getName());
        sendMessage(msg, ListOfClients[index_of_client]->getSocket());
    }
}

QString ServerSocket::getNameCurrentClient()
{
    return m_nameClientWithCurrentDialog;
}
