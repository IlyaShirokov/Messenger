#include "serverwindow.h"
#include "ui_serverwindow.h"

ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ServerWindow)
{
    ui->setupUi(this);

    connect(&server, &ServerSocket::updateStatus_log, this, &ServerWindow::updateTextBrowser_log);
}

ServerWindow::~ServerWindow()
{
    delete ui;
}


void ServerWindow::on_button_startServer_clicked()
{
    server.init();
}

void ServerWindow::on_button_stopServer_clicked()
{

}

void ServerWindow::on_button_send_clicked()
{

}

void ServerWindow::updateTextBrowser_log(QString message) //слот для обновления информации о логе сервера
{
    ui->textBrowser_log->append(message);
}

