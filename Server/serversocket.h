#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDateTime>
#include <QListWidgetItem>
#include <QFile>

#include "activeclient.h"

class ServerSocket : public QObject
{
    Q_OBJECT
public:
    ServerSocket();
    ~ServerSocket();
    void init();  //действия при старте сервера и нажати на кнопку старт сервер
    void onNewConnection(); //действия при подключении клиента
    void slotUpdateStatus_log(QString status); //слот для отправки сообщения mainWindows для записи его textBrowser лог сервера
    void slotServerRead(); //чтение данных на сокете
    void slotClientDisconnected(); //действия при отключении клиента
    void slotUpdateStatus_messages(QStringList messages);  //слот для отправки сообщения mainWindows для записи его textBrowser сообщения от пользоватлей
    void sendMessage(QString msg); //получение сообщения от mainWindows и отправка его на сокет
    void onStopingClicked();
    void slotSendNEWClientToMainWindow(QString client); //слот для отправки подключившегося клиента
    void slotSendDISCONNECTClientToMainWindow(QString client); //слот для отправки отключившегося клиента
    void selectCurrentClient(QString arg_name); //устанавливает текущего клиента для общения
    void sendClientfromMainWindow(QStringList test);

signals:
    void updateStatus_log(QString status);
    void updateStatus_messages(QStringList messages);
    void sendNEWClientToMainWindow(QString client);
    void sendDISCONNECTClientToMainWindow(QString client);

private:
    QTcpServer* m_server;
    quint16     m_port;
    int         m_serverStatus; //статус QTcpServer, чтоб не происходило эксцессов при работе сервера (если 0 — то сервер не слушает порт, 1 — слушает).

    enum {
        NotListening = 0,
        Listening = 1
    };

    QVector<ActiveClient*> SClients;
    QTcpSocket* currentClient; //клиент, которому сервер в текущий момент отправляет сообщения
};

#endif // SERVERSOCKET_H
