#include "serversocket.h"

ServerSocket::ServerSocket(){}

ServerSocket::~ServerSocket()
{
    onStopingClicked();
}

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
            ActiveClient* activeClient = new ActiveClient(m_socket->socketDescriptor(), m_socket);
            activeClient->setDialogSocket(nullptr);
            ListOfClients.append(activeClient);

            connect(ListOfClients.last()->getSocket(), &QTcpSocket::readyRead, this, &ServerSocket::slotServerRead);
            connect(ListOfClients.last()->getSocket(), &QTcpSocket::disconnected, this, &ServerSocket::slotClientDisconnected);

            ListOfClients.last()->addToHistoryOfMessages("<nobr><font color=\"green\">Сonnected to server<br></nobr>");
            slotUpdateStatus_log("<nobr><font color=\"green\">New client connected</nobr>");
        }
        else
        {
            slotUpdateStatus_log("<nobr><font color=\"red\">New client tried to connect, but it was unsuccessful</nobr>");
        }
    }
}

void ServerSocket::slotServerRead()
{
    QString     data;
    QTcpSocket* clientSocket = (QTcpSocket*)sender(); // Получаем объект сокета, который вызвал данный слот
    int index_of_client;
    for(index_of_client = 0; index_of_client < ListOfClients.length(); index_of_client++)
    {
        if (ListOfClients[index_of_client]->getSocket() == clientSocket)
            break;
    }

    while (ListOfClients[index_of_client]->getSocket()->bytesAvailable() > 0)
    {
        data = ListOfClients[index_of_client]->getSocket()->readAll();
        if (data[0] == "0") //"0" - отправляет клиент в начале строки при отправке имени
        {
            data.remove(0,1);
            slotUpdateStatus_log("<nobr><font color=\"green\">Got hostname: </nobr>" + data);
            ListOfClients[index_of_client]->setName(data);
            slotSendNEWClientToMainWindow(data);
        }
        else if (data[0] == "1") //"1" - отправляет клиент при отправке любого сообщения в  чате
        {
           data.remove(0,1);

           if (ListOfClients[index_of_client]->getDialogSocket() == nullptr)
           {
               ListOfClients[index_of_client]->addToHistoryOfMessages(ListOfClients[index_of_client]->getName() + " to Server: " + data);

               if (currentClient == ListOfClients[index_of_client]->getSocket())
                   slotUpdateStatus_messages(ListOfClients[index_of_client]->getHistoryOfMessages());
           }
           else
           {
               int dialogWith;
               for(dialogWith = 0; dialogWith < ListOfClients.length(); dialogWith++)
               {
                   if (ListOfClients[dialogWith]->getSocket() == ListOfClients[index_of_client]->getDialogSocket())
                       break;
               }

               ListOfClients[index_of_client]->addToHistoryOfMessages(ListOfClients[index_of_client]->getName() + " to " + ListOfClients[dialogWith]->getName() + ": " + data);
               ListOfClients[index_of_client]->getDialogSocket()->write("1" + data.toUtf8());
           }
        }
        else if (data[0] == "2") //"2" - отправляет клиент при отправке имени другого клиента с которым хочет начать общение
        {
            data.remove(0,1);

            int dialogWith;
            for(dialogWith = 0; dialogWith < ListOfClients.length(); dialogWith++)
            {
                if (ListOfClients[dialogWith]->getName() == data)
                    break;
            }
            ListOfClients[index_of_client]->setDialogSocket(ListOfClients[dialogWith]->getSocket());
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

    slotUpdateStatus_log("<nobr><font color=\"red\">Client \"" + ListOfClients[index_of_client]->getName() + "\" disconnected </nobr>");
    ListOfClients[index_of_client]->getSocket()->close();
    slotSendDISCONNECTClientToMainWindow(ListOfClients[index_of_client]->getName());
    ListOfClients.remove(index_of_client);
}

void ServerSocket::slotUpdateStatus_messages(QStringList messages)
{
    emit updateStatus_messages(messages);
}

void ServerSocket::slotUpdateStatus_log(QString status)
{
    emit updateStatus_log(status);
}

void ServerSocket::sendMessage(QString msg)
{
    int index_of_client;
    for(index_of_client = 0; index_of_client < ListOfClients.length(); index_of_client++)
    {
        if (ListOfClients[index_of_client]->getSocket() == currentClient)
            break;
    }

    currentClient->write("1" + msg.toUtf8());
    ListOfClients[index_of_client]->addToHistoryOfMessages("Server: " + msg);

    if (currentClient == ListOfClients[index_of_client]->getSocket())
        slotUpdateStatus_messages(ListOfClients[index_of_client]->getHistoryOfMessages());
}

void ServerSocket::onStopingClicked()
{
    if(m_serverStatus == Listening)
    {
        for(int i = 0; i < ListOfClients.length(); i++)
        {
            QTextStream os(ListOfClients[i]->getSocket());
            os.setAutoDetectUnicode(true);
            os << "1<nobr><font color=\"red\">Server closed at " << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "</nobr>";
            ListOfClients[i]->getSocket()->close();
            slotSendDISCONNECTClientToMainWindow(ListOfClients[i]->getName());
        }
        m_server->close();
        m_serverStatus = NotListening;
        slotUpdateStatus_log("<nobr><font color=\"red\">Server stopped</nobr>");
    }
}

void ServerSocket::slotSendNEWClientToMainWindow(QString client)
{
    emit sendNEWClientToMainWindow(client);
}

void ServerSocket::slotSendDISCONNECTClientToMainWindow(QString client)
{
    emit sendDISCONNECTClientToMainWindow(client);
}

void ServerSocket::selectCurrentClient(QString arg_name)
{
    int index_of_client;
    for(index_of_client = 0; index_of_client < ListOfClients.length(); index_of_client++)
    {
        if (ListOfClients[index_of_client]->getName() == arg_name)
            break;
    }

    slotUpdateStatus_messages(ListOfClients[index_of_client]->getHistoryOfMessages());

    currentClient = ListOfClients[index_of_client]->getSocket();
}

void ServerSocket::sendClientfromMainWindow(QStringList test)
{
    QString stringofClients = "";
    for (int i = 0; i < test.length(); i++)
    {
        stringofClients += test[i];
        stringofClients += ";";
    }

    for(int i = 0; i < ListOfClients.length(); i++)
    {
        QTextStream os(ListOfClients[i]->getSocket());
        os.setAutoDetectUnicode(true);
        os << "0" << stringofClients;
    }
}
