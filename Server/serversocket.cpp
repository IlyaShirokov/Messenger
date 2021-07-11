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
            SClients.append(activeClient);

            connect(SClients.last()->getSocket(), &QTcpSocket::readyRead, this, &ServerSocket::slotServerRead);
            connect(SClients.last()->getSocket(), &QTcpSocket::disconnected, this, &ServerSocket::slotClientDisconnected);

            SClients.last()->addToHistoryOfMessages("<nobr><font color=\"green\">Сonnected to server<br></nobr>");
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
    for(index_of_client = 0; index_of_client < SClients.length(); index_of_client++)
    {
        if (SClients[index_of_client]->getSocket() == clientSocket)
            break;
    }

    while (SClients[index_of_client]->getSocket()->bytesAvailable() > 0)
    {
        data = SClients[index_of_client]->getSocket()->readAll();
        if (data[0] == "0") //"0" - отправляет клиент в начале строки при отправке имени
        {
            data.remove(0,1);
            slotUpdateStatus_log("<nobr><font color=\"green\">Got hostname: </nobr>" + data);
            SClients[index_of_client]->setName(data);
            slotSendNEWClientToMainWindow(data);
        }
        else if (data[0] == "1") //"1" - отправляет клиент при отправке любого сообщения в  чате
        {
           data.remove(0,1);

           if (SClients[index_of_client]->getDialogSocket() == nullptr)
           {
               SClients[index_of_client]->addToHistoryOfMessages(SClients[index_of_client]->getName() + " to Server: " + data);

               if (currentClient == SClients[index_of_client]->getSocket())
                   slotUpdateStatus_messages(SClients[index_of_client]->getHistoryOfMessages());
           }
           else
           {
               int dialogWith;
               for(dialogWith = 0; dialogWith < SClients.length(); dialogWith++)
               {
                   if (SClients[dialogWith]->getSocket() == SClients[index_of_client]->getDialogSocket())
                       break;
               }

               SClients[index_of_client]->addToHistoryOfMessages(SClients[index_of_client]->getName() + " to " + SClients[dialogWith]->getName() + ": " + data);
               SClients[index_of_client]->getDialogSocket()->write("1" + data.toUtf8());
           }
        }
        else if (data[0] == "2") //"2" - отправляет клиент при отправке имени другого клиента с которым хочет начать общение
        {
            data.remove(0,1);

            int dialogWith;
            for(dialogWith = 0; dialogWith < SClients.length(); dialogWith++)
            {
                if (SClients[dialogWith]->getName() == data)
                    break;
            }
            SClients[index_of_client]->setDialogSocket(SClients[dialogWith]->getSocket());
        }
        else if (data[0] == "3") //отправляет главный клиент, который генерирует данные, при это у всех остальных клиентов должна заблокироваться кнопка генерировании даты и чекбокс
        {
            data.remove(0,1);

            for(int i = 0; i < SClients.length(); i++)
            {
                if (SClients[i]->getSocket() == clientSocket)
                    slotUpdateStatus_log(SClients[i]->getName() + " became a master client");
                else
                    SClients[i]->getSocket()->write("2block");
            }
        }
        else if (data[0] == "4") //отправляет клиент в начале строки при отправке QStringList. Сервер должен получить его и отправить каждому клиенту из вектора по строке
        {
            data.remove(0,1);

            QStringList list = data.split(';');

            for(int i = 0; i < SClients.length(); i++)
            {
                SClients[i]->getSocket()->write("3" + list[i].toUtf8());
            }
        }
        else if (data[0] == "5") //отправляет клиент в начале строки при отправке обработанной строки для записи ее в бд
        {
            data.remove(0,1);

            QFile fileDB;
            fileDB.setFileName("Database.txt");
            if (fileDB.open(QIODevice::Append | QIODevice::Text))
            {
                slotUpdateStatus_log("<nobr><font color=\"green\">" + SClients[index_of_client]->getName() + " opened a file (database)</nobr>");
                fileDB.write(data.toUtf8() + "\n");
            }
            else
                slotUpdateStatus_log("<nobr><font color=\"red\">" + SClients[index_of_client]->getName() + " could't open a file (database)</nobr>");

            fileDB.close();
        }
    }
}
void ServerSocket::slotClientDisconnected()
{
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    int index_of_client;
    for(index_of_client = 0; index_of_client < SClients.length(); index_of_client++)
    {
        if (SClients[index_of_client]->getSocket() == clientSocket)
            break;
    }

    slotUpdateStatus_log("<nobr><font color=\"red\">Client \"" + SClients[index_of_client]->getName() + "\" disconnected </nobr>");
    SClients[index_of_client]->getSocket()->close();
    slotSendDISCONNECTClientToMainWindow(SClients[index_of_client]->getName());
    SClients.remove(index_of_client);
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
    for(index_of_client = 0; index_of_client < SClients.length(); index_of_client++)
    {
        if (SClients[index_of_client]->getSocket() == currentClient)
            break;
    }

    currentClient->write("1" + msg.toUtf8());
    SClients[index_of_client]->addToHistoryOfMessages("Server: " + msg);

    if (currentClient == SClients[index_of_client]->getSocket())
        slotUpdateStatus_messages(SClients[index_of_client]->getHistoryOfMessages());
}

void ServerSocket::onStopingClicked()
{
    if(m_serverStatus == Listening)
    {
        for(int i = 0; i < SClients.length(); i++)
        {
            QTextStream os(SClients[i]->getSocket());
            os.setAutoDetectUnicode(true);
            os << "1<nobr><font color=\"red\">Server closed at " << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "</nobr>";
            SClients[i]->getSocket()->close();
            slotSendDISCONNECTClientToMainWindow(SClients[i]->getName());
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
    for(index_of_client = 0; index_of_client < SClients.length(); index_of_client++)
    {
        if (SClients[index_of_client]->getName() == arg_name)
            break;
    }

    slotUpdateStatus_messages(SClients[index_of_client]->getHistoryOfMessages());

    currentClient = SClients[index_of_client]->getSocket();
}

void ServerSocket::sendClientfromMainWindow(QStringList test)
{
    QString stringofClients = "";
    for (int i = 0; i < test.length(); i++)
    {
        stringofClients += test[i];
        stringofClients += ";";
    }

    for(int i = 0; i < SClients.length(); i++)
    {
        QTextStream os(SClients[i]->getSocket());
        os.setAutoDetectUnicode(true);
        os << "0" << stringofClients;
    }
}
