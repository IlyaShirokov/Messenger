#ifndef ACTIVECLIENT_H
#define ACTIVECLIENT_H

#include <QObject>
#include <QTcpSocket>

class ActiveClient : public QObject
{
    Q_OBJECT
public:
    ActiveClient(QObject *parent = nullptr);
    ActiveClient(qintptr arg_decriptor, QTcpSocket* arg_socket, QString arg_name = "noname");

    void setName(QString arg_name);
    void setDescriptorSocket(qintptr arg_descriptor);
    void setSocket(QTcpSocket* arg_socket);
    void addToHistoryOfMessages(QString msg);
    void setDialogSocket(QTcpSocket* arg_socket);

    QString getName();
    qintptr getDescriptor();
    QTcpSocket* getSocket();
    QStringList getHistoryOfMessages();
    QTcpSocket* getDialogSocket();

signals:

private:
    qintptr m_descriptor_socket;
    QString username;
    QTcpSocket* m_socket;
    QStringList historyOfMessages;
    QTcpSocket* currentSocketDialogWithUser;

};

#endif // ACTIVECLIENT_H
