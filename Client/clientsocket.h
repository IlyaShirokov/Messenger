#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QDateTime>

class ClientSocket : public QObject
{
    Q_OBJECT
public:
    explicit ClientSocket(QObject *parent = nullptr);
    void newConnect(); //создание нового подключения к серверу
    void onConnected();  //чтение данных сокета
    void slot_updateStatus(QString status);  //слот для обновления данных в textBrowser
    void sockDisconnected();  //действия при отключении от сервера
    void sockRead();
    void sendMessage(QString msg);  //отправка сообщения при нажатии на кнопку Send
    void setName(QString arg_name);
    QString getName();
    void updateActiveClient(QString arg_data); //получение списка активных клиентов
    void setNameClientWithCurrentDialog(QString arg_name);
    QString getNameClientWithCurrentDialog();
    void selectUserForDialog(QString arg_user);

signals:
    void updateStatus(QString status);  //сигнал, необходимый для отправки в mainWindow текста в textBrowser
    void updateClients(QStringList list); //стгнал, необходимый для отправки в mainWindow списка активных клиентов в widgetList
    void blockCheckBoxAndButton();
    void setStrForProcessing(QString str);
    void setStrFinalStr(QString str);

private:
    QTcpSocket* m_socket;
    quint16     m_port;
    QString     data;
    QString     m_NameOfUser;
    QString     nameClientWithCurrentDialog;

};

#endif // CLIENTSOCKET_H
