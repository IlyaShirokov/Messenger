#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include "serversocket.h"

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
    void on_button_startServer_clicked();  //действие при нажати на кнопку запуска сервера
    void on_button_stopServer_clicked(); //действие при нажати на кнопку стоп сервера
    void on_button_send_clicked(); //действия при нажатии на отправку сообщения
    void updateTextBrowser_log(QString message); //слот для обновления информации о логе сервера
    void updateTextBrowser_messages(QStringList messages); // слот для обновления информации о сообщениях
    void updateListActiveNEWClients(QString arg_Client); //слот для обновления информации о новых активных подключениях
    void updateListActiveDISCONNECTClients(QString arg_Client);//слот для обновления информации о отключившихся подключениях
    void changeTitleGroupBox(); //слот для изменения имени групбокса
    void changeCurrentClient(); // вызывается при выборе нового клиента из списка подключений с которым вести общение

private:
    Ui::ServerWindow *ui;
    ServerSocket server;
    QStringList currentListOfUsers;
};
#endif // SERVERWINDOW_H
