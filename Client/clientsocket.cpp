#include "clientsocket.h"

ClientSocket::ClientSocket(QObject *parent) : QObject(parent){}

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
    slot_updateStatus("<nobr><font color=\"green\">Successfuly connected to server<br></nobr>");
    m_socket->write("0" + m_NameOfUser.toUtf8()); //добаляем 0 в начале для обозначчения служебного сообщения
    connect(m_socket, &QTcpSocket::readyRead, this, &ClientSocket::sockRead);
    connect(m_socket, &QTcpSocket::disconnected, this, &ClientSocket::sockDisconnected);
}

void ClientSocket::slot_updateStatus(QString status)
{
    emit updateStatus(status);
}

void ClientSocket::sockDisconnected()
{
    m_socket->deleteLater();
}

void ClientSocket::sockRead()
{
    if (m_socket->waitForConnected(500))
    {
        m_socket->waitForReadyRead(500);
        data = m_socket->readAll();
        if (data[0] == "1") // отправляет сервер при отправке любого сообщения
        {
            data = data.remove(0,1);
            slot_updateStatus(data);
        }
        else if (data[0] == "0") //отправляет сервер при отправке списка клиентов каждому из клиентов
        {
            data = data.remove(0,1);
            updateActiveClient(data);
        }
    }
}

void ClientSocket::sendMessage(QString msg)
{
    m_socket->write(msg.toUtf8());
}

void ClientSocket::setName(QString arg_name)
{
    m_NameOfUser = arg_name;
}

QString ClientSocket::getName()
{
    return m_NameOfUser;
}

void ClientSocket::updateActiveClient(QString arg_data)
{
    QStringList list = arg_data.split(';');
    emit updateClients(list);
}

void ClientSocket::setNameClientWithCurrentDialog(QString arg_name)
{
    nameClientWithCurrentDialog = arg_name;
}

QString ClientSocket::getNameClientWithCurrentDialog()
{
    return nameClientWithCurrentDialog;
}
void ClientSocket::selectUserForDialog(QString arg_user)
{
    m_socket->write("2" + arg_user.toUtf8());
}
