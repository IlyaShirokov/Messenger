#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>

#include <serversocket.h>

QT_BEGIN_NAMESPACE
namespace Ui { class ServerWindow; }
QT_END_NAMESPACE

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();

private slots:
    void on_button_startServer_clicked();                       //действия при нажатии на кнопку "старт сервер"
    void on_button_stopServer_clicked();                        //действия при нажатии на кнопку "стоп сервер"
    void updateTextBrowser_log(QString message);                //слот для обновления информации о логе сервера
    void updateTextBrowser_messages(QStringList messages);      //слот для обновления информации о сообщениях
    void updateListActiveClients(QStringList arg_Clients);      //слот для обновления активных подключениях
    void changeCurrentClient();                                 // вызывается при выборе нового клиента из списка подключений с которым вести общение
    void checkIpAndPort();                                      //проверка корректности введеного адреса и порта
    void setButtonSendEnabled(int state);                       //активация кнопки отправки при нажатии галочки "отправить всем"
    void on_button_sendMsg_clicked();                           //действия при нажатии на кнопку отправки сообщения

private:
    Ui::ServerWindow *ui;
    ServerSocket server;
    QStringList currentListOfClients;           //список активных клиентов
    bool loadListOfClient;                      //становится единицей при первой загрузке списка клиентов при подключении
};
#endif // SERVERWINDOW_H
