#include "clientsocket.h"

ClientSocket::ClientSocket(QObject *parent) : QObject(parent)
{
    registrationRequest = false;
}

void ClientSocket::setConnect(QString name, QString passwd, QString ipAddres, qint16 port)
{
    m_port = port;
    m_password  = passwd;
    m_NameOfUser = name;
    m_nameClientWithCurrentDialog = "";
    m_socket = new QTcpSocket(this);
    m_socket->connectToHost(ipAddres, m_port);
    connect(m_socket, &QTcpSocket::connected, this, &ClientSocket::onConnected);
}

void ClientSocket::onConnected()
{
    if (registrationRequest)
    {
        Message msg(m_password, Message::comRegistrationRequest, m_NameOfUser, m_nameClientWithCurrentDialog);
        sendMessage(msg);
    }
    else
    {
        Message msg(m_password, Message::comAutchRequest, m_NameOfUser, m_nameClientWithCurrentDialog);
        sendMessage(msg);
    }
    connect(m_socket, &QTcpSocket::readyRead, this, &ClientSocket::socketRead);
    connect(m_socket, &QTcpSocket::disconnected, this, &ClientSocket::socketDisconnected);
}

void ClientSocket::socketDisconnected()
{
    emit serverDisconnected();
    m_socket->deleteLater();
}

void ClientSocket::socketRead()
{
    QDataStream stream_in(m_socket);
    Message msg_in;
    m_blockSize = 0;

    if(m_blockSize == 0)
    {
        while (m_socket->bytesAvailable() < (int)sizeof(quint16))
        {
            continue;
        }
        stream_in >> m_blockSize;
    }

    while (m_socket->bytesAvailable() < m_blockSize)
    {
        continue;
    }
    m_blockSize = 0;

    stream_in >> msg_in;

    switch (msg_in.getTypeMessage())
    {
    case Message::comUsersOnline:
    {
       updateActiveClient(msg_in.getTextData());
       break;
    }
    case Message::comTextMessage:
    {
        QString senderAndData = msg_in.getSenderName() + ';' + msg_in.getTextData();
        emit updateMessages(senderAndData);
        break;
    }
    case Message::comServerClosed:
    {
        m_socket->deleteLater();
        break;
    }
    case Message::comSuccessfulAuth:
    {
       emit displayClientWindow();
       emit updateMessages("Server;<nobr><font color=\"green\">Successfuly connected to server<br></nobr>");
       break;
    }
    case Message::comDeclineAuth:
    {
       emit loginUserAnswerDB(false);
       m_socket->deleteLater();
       break;
    }
    case Message::comSuccessfulRegistration:
    {
        emit registerUserAnswerDB(true);
        m_socket->deleteLater();
       break;
    }
    case Message::comDeclineRegistration:
    {
       emit registerUserAnswerDB(false);
       m_socket->deleteLater();
       break;
    }
    }
}

void ClientSocket::sendMessage(Message &msg)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << msg;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    m_socket->write(block);
}

void ClientSocket::updateActiveClient(QString arg_data)
{
    QStringList list = arg_data.split(';');
    emit updateClients(list);
}

QString ClientSocket::getNameClientWithCurrentDialog()
{
    return m_nameClientWithCurrentDialog;
}

void ClientSocket::getMessageFromWindow(QString data)
{
    Message msg(data, Message::comTextMessage, m_NameOfUser, m_nameClientWithCurrentDialog);
    sendMessage(msg);
}

void ClientSocket::selectUserForDialog(QString arg_user)
{
    m_nameClientWithCurrentDialog = arg_user;
    Message msg(arg_user, Message::comStartDialogWithUser, m_NameOfUser, m_nameClientWithCurrentDialog);
    sendMessage(msg);
}

QString ClientSocket::getName()
{
    return m_NameOfUser;
}

QString ClientSocket::getNameClientDialogWith()
{
    return m_nameClientWithCurrentDialog;
}

void ClientSocket::registerClient(QString name, QString passwd, QString ipAddres, qint16 port)
{
    registrationRequest = true;
    setConnect(name, passwd, ipAddres, port);
}
