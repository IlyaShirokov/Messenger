#include "authentication.h"
#include "ui_authentication.h"

Authentication::Authentication(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Authentication)
{
    ui->setupUi(this);

    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    ui->lineEdit_registerPassword->setEchoMode(QLineEdit::Password);
    ui->lineEdit_registerConfirm->setEchoMode(QLineEdit::Password);
    ui->stackedWidget->setCurrentIndex(0);
    ui->button_login->setEnabled(0);
    ui->button_Register->setEnabled(0);

    m_ipAddress = "127.0.0.1";
    m_port = 5555;

    connect(ui->button_Register, &QPushButton::clicked, this, &Authentication::registerUsertoDB);
    connect(ui->button_applySetting, &QPushButton::clicked, this, &Authentication::applySetting);
    connect(ui->button_backToLogin, &QPushButton::clicked, this, &Authentication::goToLogIn);
    connect(ui->button_backToLogin_2, &QPushButton::clicked, this, &Authentication::goToLogIn);
    connect(ui->button_goToRegister, &QPushButton::clicked, this, &Authentication::goToRegister);
    connect(ui->button_login, &QPushButton::clicked, this, &Authentication::loginUsertoDB);
    connect(ui->button_setting, &QPushButton::clicked, this, &Authentication::goToSetting);
    connect(ui->checkBox_showRegisterPasswd, &QCheckBox::stateChanged, this, &Authentication::showRegisterPasswd);
    connect(ui->checkBox_showLoginPasswd, &QCheckBox::stateChanged, this, &Authentication::showLoginPasswd);
    connect(ui->lineEdit_login, &QLineEdit::textEdited, this, &Authentication::checkLogin);
    connect(ui->lineEdit_password, &QLineEdit::textEdited, this, &Authentication::checkLogin);
    connect(ui->lineEdit_username, &QLineEdit::textEdited, this, &Authentication::checkRegistration);
    connect(ui->lineEdit_registerPassword, &QLineEdit::textEdited, this, &Authentication::checkRegistration);
    connect(ui->lineEdit_registerConfirm, &QLineEdit::textEdited, this, &Authentication::checkRegistration);
}

Authentication::~Authentication()
{
    delete ui;
}

void Authentication::goToRegister()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Authentication::goToLogIn()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Authentication::goToSetting()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void Authentication::applySetting()
{
    m_ipAddress = ui->lineEdit_ipAddress->text();
    m_port = ui->lineEdit_port->text().toInt();
    goToLogIn();
}

void Authentication::registerUsertoDB()
{
    if (ui->lineEdit_registerPassword->text() == ui->lineEdit_registerConfirm->text())
        emit sendRegister(ui->lineEdit_username->text(), ui->lineEdit_registerPassword->text(), m_ipAddress, m_port);
}

void Authentication::loginUsertoDB()
{
    emit sendLogin(ui->lineEdit_login->text(), ui->lineEdit_password->text(), m_ipAddress, m_port);
}

void Authentication::showRegisterPasswd(int state)
{
    if (state == Qt::Unchecked)
    {
        ui->lineEdit_registerPassword->setEchoMode(QLineEdit::Password);
        ui->lineEdit_registerConfirm->setEchoMode(QLineEdit::Password);
    }
    else
    {
        ui->lineEdit_registerPassword->setEchoMode(QLineEdit::Normal);
        ui->lineEdit_registerConfirm->setEchoMode(QLineEdit::Normal);
    }
}

void Authentication::showLoginPasswd(int state)
{
    if (state == Qt::Unchecked)
    {
        ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    }
    else
    {
        ui->lineEdit_password->setEchoMode(QLineEdit::Normal);
    }
}

void Authentication::loginUserAnswerDB(bool ans)
{
    if (!ans)
    {
        ui->label_responseFromDBlogin->setText("<font color=\"red\">Wrong username / password");
    }
}

void Authentication::registerUserAnswerDB(bool ans)
{
    if (ans)
    {
        goToLogIn();
    }
    else
    {
        ui->label_responseFromDBregister->setText("<font color=\"red\">Unsuccessful registration");
    }
}

void Authentication::checkRegistration()
{
    if ((ui->lineEdit_username->text() != "") && (ui->lineEdit_registerPassword->text() != "") && (ui->lineEdit_registerConfirm->text() != ""))
        ui->button_Register->setEnabled(1);
    else
        ui->button_Register->setEnabled(0);
}

void Authentication::checkLogin()
{
    if ((ui->lineEdit_login->text() != "") && (ui->lineEdit_password->text() != ""))
        ui->button_login->setEnabled(1);
    else
        ui->button_login->setEnabled(0);
}
