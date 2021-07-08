#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class ServerSocket : public QObject
{
    Q_OBJECT
public:
    ServerSocket();
    void init();  //действия при старте сервера и нажати на кнопку старт сервер
    void onNewConnection(); //действия при подключении клиента
    void slotUpdateStatus_log(QString status); //слот для отправки сообщения mainWindows для записи его textBrowser лог сервера

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
};

#endif // SERVERSOCKET_H
