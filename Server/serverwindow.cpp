#include "serverwindow.h"
#include "ui_serverwindow.h"

ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ServerWindow)
{
    ui->setupUi(this);

    connect(&server, &ServerSocket::updateStatus_log, this, &ServerWindow::updateTextBrowser_log);
    connect(&server, &ServerSocket::updateStatus_messages, this, &ServerWindow::updateTextBrowser_messages);
    connect(&server, &ServerSocket::sendNEWClientToMainWindow, this, &ServerWindow::updateListActiveNEWClients);
    connect(&server, &ServerSocket::sendDISCONNECTClientToMainWindow, this, &ServerWindow::updateListActiveDISCONNECTClients);
    connect(ui->listWidget_connections, &QListWidget::doubleClicked, this, &ServerWindow::changeTitleGroupBox);
    connect(ui->listWidget_connections, &QListWidget::doubleClicked, this, &ServerWindow::changeCurrentClient);
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
    server.onStopingClicked();
}

void ServerWindow::on_button_send_clicked()
{
    server.sendMessage(ui->textEdit_message->toPlainText());
    ui->textEdit_message->clear();
}

void ServerWindow::updateTextBrowser_log(QString message) //слот для обновления информации о логе сервера
{
    ui->textBrowser_log->append(message);
}

void ServerWindow::updateTextBrowser_messages(QStringList messages)
{
    ui->textBrowser_messages->clear();
    for(int i = 0; i < messages.length(); i++)
        ui->textBrowser_messages->append(messages[i]);
}

void ServerWindow::updateListActiveNEWClients(QString arg_Client)
{
    QListWidgetItem *item = new QListWidgetItem(arg_Client);
    ui->listWidget_connections->addItem(item);
    currentListOfUsers.append(arg_Client);

    server.sendClientfromMainWindow(currentListOfUsers);
}

void ServerWindow::updateListActiveDISCONNECTClients(QString arg_Client)
{
    QString stringOnGroupBox = "Dialog with: " + arg_Client; //проверка на то, что удаляемый клинт не тот, с котором ведется текущий диалог
    QString currenttextOnGroupBox = ui->groupBox_3->title(); //если удаляется текущий клиент, то имя груп бокса сбрасывается на дефорлтное
    if (stringOnGroupBox == currenttextOnGroupBox)
        ui->groupBox_3->setTitle("Dialog with: ");

    QList<QListWidgetItem*> out;
    out = ui->listWidget_connections->findItems(arg_Client, Qt::MatchExactly);
    foreach(QListWidgetItem* item, out){
        ui->listWidget_connections->removeItemWidget(item);
        delete item; // Qt documentation warnings you to destroy item to effectively remove it from QListWidget.
    }

    for (int i = 0; i < currentListOfUsers.length(); i++)
        if (currentListOfUsers[i] == arg_Client)
            currentListOfUsers.removeAt(i);

    server.sendClientfromMainWindow(currentListOfUsers);
}

void ServerWindow::changeTitleGroupBox()
{
    ui->groupBox_3->setTitle("Dialog with: " + ui->listWidget_connections->currentItem()->text());
}

void ServerWindow::changeCurrentClient()
{
    server.selectCurrentClient(ui->listWidget_connections->currentItem()->text());
}

