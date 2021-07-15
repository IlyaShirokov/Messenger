#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QDataStream>

class Message : public QObject
{
    Q_OBJECT
public:
    explicit Message(QObject *parent = nullptr);
    Message(QString m_textData, qint8 typeMessage, QString senderName, QString destinationName);

    QString getTextData();
    qint8 getTypeMessage();
    QString getSenderName();
    QString getDestinationName();

    friend QDataStream& operator<<(QDataStream &out, const Message &msg); //оператор вывода
    friend QDataStream& operator>>(QDataStream &in, Message &msg);       //оператор ввода

    static const quint8 comAutchRequest = 0;            //отправляет клиент при логине
    static const quint8 comUsersOnline = 1;             //отправляет сервер при отправке списка пользователей, которые онлайн
    static const quint8 comTextMessage = 2;             //отправляет клиент/сервер при отправке любого текстового сообщения
    static const quint8 comStartDialogWithUser = 3;     //отправляет клиент вместе с именем пользователя, с которым хочет начать диалог
    static const quint8 comServerClosed = 4;            //отправляет сервер всем клиентам при нажатии на кнопку стоп сервер
    static const quint8 comSuccessfulAuth = 5;          //отправляет сервер при успешной авторизации клиента
    static const quint8 comDeclineAuth = 6;             //отправляет сервер при неудачной авторизации
    static const quint8 comRegistrationRequest = 7;     //отправляет клиент при запросе регистрации
    static const quint8 comSuccessfulRegistration = 8;  //отправляет сервер при успешной авторизации клиента
    static const quint8 comDeclineRegistration = 9;     //отправляет сервер при неудачной авторизации

signals:

private:
    QString m_textData;         //текстовые данные сообщения
    qint8 m_typeMessage;        //тип сообщения
    QString m_senderName;       //имя отправителя
    QString m_destinationName;  //имя получателя


};

#endif // MESSAGE_H
