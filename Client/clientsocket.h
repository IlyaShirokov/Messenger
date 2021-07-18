#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include "..\\Common\\message.h"

class ClientSocket : public QObject
{
    Q_OBJECT
public:
    explicit ClientSocket(QObject *parent = nullptr);
    void setConnect(QString name, QString passwd, QString ipAddres, qint16 port);       //действия при нажатии на кнопку подключения к серверу
    void onConnected();                                             //действия при получении ответа от сервера при успешном подключении
    void socketDisconnected();                                      //действия при потере соединения с сервером
    void socketRead();                                              //действия при получении сообщения от сервера
    void sendMessage(Message &msg);                                 //функция отправки сообщения
    void updateActiveClient(QString arg_data);                      //действия при получении с сервера списка активных клиентов
    QString getNameClientWithCurrentDialog();                       //получение имени клиента с которым сейчас активный диалог
    void getMessageFromWindow(QString msg);                         //получение сообщения из поля главного окна
    void selectUserForDialog(QString arg_user);                     //выбор пользователя с которым будет вестись диалог
    QString getName();                                              //получение имени текущего клиента
    QString getNameClientDialogWith();                              //получение имени клиента с которым будет вестись диалог
    void registerClient(QString name, QString passwd, QString ipAddres, qint16 port);   //отправка серу запроса на регистрацию

signals:
    void updateMessages(QString message);                                   //сигнал, необходимый для отправки в ClientWindow текста в textBrowser_messages
    void updateClients(QStringList list);                                   //сигнал, необходимый для отправки в mainWindow списка активных клиентов в widgetList
    void serverDisconnected(bool glagAuthorizated);                         //сигнал главному окну, о том, что сервер закрыт
    void displayClientWindow(QString name, QString ipAddres, qint16 port);  //сигнал главному окну после успешного логина
    void declineConnect();                                                  //сигнал окну авторизации, что подключение не удалось
    void registerUserAnswerDB(bool ans);
    void loginUserAnswerDB(bool ans);
    void addToHistoryMsgMainWindow(Message msg);

private:
    QTcpSocket* m_socket;                       //объект сокета
    qint16      m_port;                         //используемый порт
    QString     m_ipAddress;
    QString     m_NameOfUser;                   //имя клиента
    QString     m_password;                     //пароль пользователя
    QString     m_nameClientWithCurrentDialog;  //имя клиента с которым ведется диалог
    quint16     m_blockSize;                    //текущий размер блока данных
    bool registrationRequest;                   //флаг обозначающий запрос на регистрацию, нужен для того чтобы после подключения отправить правильный код
    bool authorizatesNow;                       //флаг необходимый для проверки того авторизовались мы или еще нет

};

#endif // CLIENTSOCKET_H
