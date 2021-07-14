#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include "clientsocket.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ClientWindow; }
QT_END_NAMESPACE

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    ClientWindow(QWidget *parent = nullptr);
    ~ClientWindow();

private slots:
    void on_button_connectToServer_clicked();           //действия при нажатии на кнопку подключения к серверу
    void on_button_sendMessage_clicked();               //действия при нажатии на кнопку отправки сообщения
    void updateTextBrowser_messages(QString msg);       //обновление списка сообщений
    void checkName();                                   //проверка корректности введеного имени
    void changeUsersInList(QStringList arg_data);       //обновление списка активных пользователей
    void changeUserDialogWith();                        //смена пользователя с которым ведется диалог
    void serverDisconnected();                          //действия при потере соединения с сервером

private:
    Ui::ClientWindow *ui;
    ClientSocket client;
    QStringList currentListOfClients;       //список активных клиентов
    bool loadListOfClient;                  //становится единицей при первой загрузке списка клиентов при подключении
};
#endif // CLIENTWINDOW_H
