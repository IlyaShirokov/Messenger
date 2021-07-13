#include "clientsocket.h"

ClientSocket::ClientSocket(QObject *parent) : QObject(parent){}

//void ClientSocket::slot_updateStatus(QString status)
//{
//    emit updateStatus(status);
//}

//void ClientSocket::sendMessage(QString msg)
//{
//    m_socket->write(msg.toUtf8());
//}

//void ClientSocket::updateActiveClient(QString arg_data)
//{
//    QStringList list = arg_data.split(';');
//    emit updateClients(list);
//}

//void ClientSocket::setNameClientWithCurrentDialog(QString arg_name)
//{
//    nameClientWithCurrentDialog = arg_name;
//}

//QString ClientSocket::getNameClientWithCurrentDialog()
//{
//    return nameClientWithCurrentDialog;
//}
//void ClientSocket::selectUserForDialog(QString arg_user)
//{
//    m_socket->write("2" + arg_user.toUtf8());
//}

void ClientSocket::setName(QString arg_name)
{
    m_NameOfUser = arg_name;
}

QString ClientSocket::getName()
{
    return m_NameOfUser;
}

void ClientSocket::newConnect()
{
    m_port = 5555;
    m_socket = new QTcpSocket(this);
    m_socket->connectToHost("127.0.0.1", m_port);
    connect(m_socket, &QTcpSocket::connected, this, &ClientSocket::onConnected);
    nameClientWithCurrentDialog = "";
}

void ClientSocket::onConnected()
{
    //после подключения следует отправить запрос на авторизацию
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    //резервируем 2 байта для размера блока.
    //третий байт - команда
    out << (quint16)0 << (quint8)comSendName << m_NameOfUser;
    //возваращаемся в начало
    out.device()->seek(0);
    //вписываем размер блока на зарезервированное место
    out << (quint16)(block.size() - sizeof(quint16));
    m_socket->write(block);

    slot_updateStatus("<nobr><font color=\"green\">Successfuly connected to server<br></nobr>");
    connect(m_socket, &QTcpSocket::readyRead, this, &ClientSocket::sockRead);
    connect(m_socket, &QTcpSocket::disconnected, this, &ClientSocket::sockDisconnected);
}

void ClientSocket::sockDisconnected()
{
    m_socket->deleteLater();
}

void ClientSocket::sockRead()
{
//    if (m_socket->waitForConnected(500))
//    {
//        m_socket->waitForReadyRead(500);
//        data = m_socket->readAll();
//        if (data[0] == "1") // отправляет сервер при отправке любого сообщения
//        {
//            data = data.remove(0,1);
//            slot_updateStatus(data);
//        }
//        else if (data[0] == "0") //отправляет сервер при отправке списка клиентов каждому из клиентов
//        {
//            data = data.remove(0,1);
//            updateActiveClient(data);
//        }
//    }

    QDataStream in(m_socket);
    //если считываем новый блок первые 2 байта это его размер
    if (m_blockSize == 0) {
        //если пришло меньше 2 байт ждем пока будет 2 байта
        if (m_socket->bytesAvailable() < (int)sizeof(quint16))
            return;
        //считываем размер (2 байта)
        in >> m_blockSize;
        qDebug() << "_blockSize now " << m_blockSize;
    }
    //ждем пока блок придет полностью
    if (m_socket->bytesAvailable() < m_blockSize)
        return;
    else
        //можно принимать новый блок
        m_blockSize = 0;
    //3 байт - команда серверу
    quint8 command;
    in >> command;
    qDebug() << "Received command " << command;

    switch (command)
    {
        case comGetUsersOnline:
        {

        }
    }
}
