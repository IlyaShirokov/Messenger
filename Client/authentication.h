#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <QMainWindow>

namespace Ui {
class Authentication;
}

class Authentication : public QMainWindow
{
    Q_OBJECT

public:
    explicit Authentication(QWidget *parent = nullptr);
    ~Authentication();
    void loginUserAnswerDB(bool ans);       //действия при получении ответа от базы данных при логине
    void registerUserAnswerDB(bool ans);    //действия при получении ответа от базы данных при регистрации

private slots:
    void goToRegister();                    //открывает окно с регистрацией
    void goToLogIn();                       //открывает окно с логином
    void goToSetting();                     //открывает окно с регистрацией
    void applySetting();                    //сохранение настроек
    void registerUsertoDB();                //кнопка регистрации пользователя
    void loginUsertoDB();                   //кнопка логина пользователя
    void showRegisterPasswd(int state);     //показать/скрыть пароль при регистрации
    void showLoginPasswd(int state);        //показать/скрыть пароль при логине
    void checkRegistration();               //проверка на непустые строки при регистрации
    void checkLogin();                      //проверка на непустые строки при логине

signals:
    void sendLogin(QString login, QString passwd, QString ipAddr, qint16 port);
    void sendRegister(QString login, QString passwd, QString ipAddr, qint16 port);

private:
    Ui::Authentication *ui;
    QString m_ipAddress;
    qint16 m_port;
};

#endif // AUTHENTICATION_H
