#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include "clientsocket.h"
#include "authentication.h"

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
    void on_button_sendMessage_clicked();                       //действия при нажатии на кнопку отправки сообщения
    void updateTextBrowser_messages(QString msg);               //обновление списка сообщений
    void changeUsersInList(QStringList arg_data);               //обновление списка активных пользователей
    void changeUserDialogWith();                                //смена пользователя с которым ведется диалог
    void serverDisconnected(bool flagAuthorizated);             //действия при потере соединения с сервером
    void display(QString name, QString ipAddres, qint16 port);  //отображение главного окна в случае успешной авторизации
    void on_button_logout_clicked();                            //нажатие на кнопку смены пользователя
    void addToHistoryMsg(Message msg);

private:
    Ui::ClientWindow *ui;
    Authentication auth;
    ClientSocket client;
    QStringList currentListOfClients;       //список активных клиентов
    bool loadListOfClient;                  //становится единицей при первой загрузке списка клиентов при подключении
    QVector<Message> historyOfMessage;
};
#endif // CLIENTWINDOW_H
