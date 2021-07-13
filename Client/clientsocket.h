#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QDateTime>
#include <QDataStream>

#include "message.h"

class ClientSocket : public QObject
{
    Q_OBJECT
public:
    explicit ClientSocket(QObject *parent = nullptr);
//    void sendMessage(QString msg);  //отправка сообщения при нажатии на кнопку Send
//    void updateActiveClient(QString arg_data); //получение списка активных клиентов
//    void setNameClientWithCurrentDialog(QString arg_name);
//    QString getNameClientWithCurrentDialog();
//    void selectUserForDialog(QString arg_user);

//----------------------------------------------
    void setName(QString arg_name); //запись имени в объект класса
    QString getName(); //получение имени
    void newConnect(); //создание нового поключения к серверу
    void onConnected();  //аутентификация и подключение сигналов
    void slot_updateStatus(QString status);  //слот для обновления данных в textBrowser
    void sockDisconnected();  //действия при отключении от сервера
    void sockRead();

//----------------------------------------------

signals:
    void updateStatus(QString status);  //сигнал, необходимый для отправки в mainWindow текста в textBrowser
    void updateClients(QStringList list); //стгнал, необходимый для отправки в mainWindow списка активных клиентов в widgetList

private:
    QTcpSocket* m_socket; //объект сокета
    quint16     m_port; //порт
    QString     data; //содержимое сообщения
    QString     m_NameOfUser; //имя клиента
    QString     nameClientWithCurrentDialog; //имя клиента с которым ведется диалог
    quint16     m_blockSize;//текущий размер блока данных

    const quint8 comSendName = 0;
    const quint8 comSendMessage = 1;
    const quint8 comSendUsersDialogWith = 2;
    const quint8 comGetUsersOnline = 0;
};

#endif // CLIENTSOCKET_H
