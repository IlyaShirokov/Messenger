#include "activeclient.h"

ActiveClient::ActiveClient(QObject *parent) : QObject(parent){}

ActiveClient::ActiveClient(qintptr arg_decriptor, QTcpSocket* arg_socket, QString arg_name)
{
    m_descriptor_socket = arg_decriptor;
    username = arg_name;
    m_socket = arg_socket;
}

void ActiveClient::setName(QString arg_name)
{
    username = arg_name;
}

void ActiveClient::setDescriptorSocket(qintptr arg_descriptor)
{
    m_descriptor_socket = arg_descriptor;
}

void ActiveClient::setSocket(QTcpSocket* arg_socket)
{
    m_socket = arg_socket;
}

QString ActiveClient::getName()
{
    return username;
}

qintptr ActiveClient::getDescriptor()
{
    return m_descriptor_socket;
}

QTcpSocket* ActiveClient::getSocket()
{
    return m_socket;
}

void ActiveClient::addToHistoryOfMessages(QString msg)
{
    historyOfMessages.append(msg);
}

QStringList ActiveClient::getHistoryOfMessages()
{
    return historyOfMessages;
}

void ActiveClient::setDialogSocket(QTcpSocket* arg_socket)
{
    currentSocketDialogWithUser = arg_socket;
}

QTcpSocket* ActiveClient::getDialogSocket()
{
    return currentSocketDialogWithUser;
}
