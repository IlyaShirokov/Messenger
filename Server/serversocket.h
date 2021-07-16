#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDateTime>

#include "activeclient.h"
#include "..\\Common\\message.h"
#include "..\\Common\\connectdb.h"

class ServerSocket : public QObject
{
    Q_OBJECT
public:
    explicit ServerSocket(QObject *parent = nullptr);
    ~ServerSocket();

    void initServer(QString ipAddres, qint16 port);             //действия при старте сервера и нажати на кнопку старт сервер
    void onNewConnection();                                     //действия при подключении клиента
    void slotServerRead();                                      //дейсвтия при появлении данных на сокете
    void slotClientDisconnected();                              //действия при отключении клиента
    void getMessageFromWindow(QString data);                    //получение введенного сообщения из главного окна
    void sendMessage(Message &msg, QTcpSocket* client);         //функция отправки сообщения
    void onStopingClicked();                                    //действия при нажатии на кнопку остановки сервера
    void selectCurrentClient(QString arg_name);                 //устанавливает текущего клиента для общения
    void sendListOfActiveClient(QStringList listClients);       //получение списка активных клиентов и отправка его все остальным клиентам
    void getMessageFromMainWindow(QString data, bool toAll);    //получение введенного сообщения из главного окна
    QString getNameCurrentClient();                             //получение имени клиента с которым ведется диалог
    void readHistoryOfMsgFromDB(QString sender);

signals:
    void updateStatus_log(QString status);                  //обновление лога сервера в mainWindow
    void updateStatus_messages(QStringList messages);       //передача списка сообщений в mainWindow
    void sendClientToMainWindow(QStringList clients);       //передача списка актуальных клиентов в mainWindow

private:
    QTcpServer* m_server;
    quint16     m_port;
    int         m_serverStatus;     //статус QTcpServer, чтоб не происходило эксцессов при работе сервера (если 0 — то сервер не слушает порт, 1 — слушает).

    enum {
        NotListening = 0,
        Listening = 1
    };

    QVector<ActiveClient*> ListOfClients;       //список активных клиентов
    QStringList ListNameOfClients;              //список имен активных клиентов
    QTcpSocket* currentClient;                  //клиент, которому сервер в текущий момент отправляет сообщения
    QString     m_nameClientWithCurrentDialog;  //имя клиента с которым ведется диалог
    quint16     m_blockSize;                    //текущий размер блока данных
    ConnectDB   m_db;

};

#endif // SERVERSOCKET_H
