#include "serverwindow.h"
#include "ui_serverwindow.h"

ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ServerWindow)
{
    ui->setupUi(this);

    loadListOfClient = 0;

    ui->button_stopServer->setEnabled(0);
    checkIpAndPort();
    ui->button_sendMsg->setEnabled(0);

    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange + "\\." + ipRange + "\\." + ipRange + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->lineEdit_ipAddress->setValidator(ipValidator);
    ui->lineEdit_ipAddress->setInputMask("000.000.000.000");

    connect(&server, &ServerSocket::updateStatus_log, this, &ServerWindow::updateTextBrowser_log);
    connect(&server, &ServerSocket::updateStatus_messages, this, &ServerWindow::updateTextBrowser_messages);
    connect(&server, &ServerSocket::sendClientToMainWindow, this, &ServerWindow::updateListActiveClients);
    connect(ui->listWidget_connections, &QListWidget::doubleClicked, this, &ServerWindow::changeCurrentClient);
    connect(ui->lineEdit_port, &QLineEdit::textEdited, this, &ServerWindow::checkIpAndPort);
    connect(ui->lineEdit_ipAddress, &QLineEdit::textEdited, this, &ServerWindow::checkIpAndPort);
    connect(ui->checkBox, &QCheckBox::stateChanged, this, &ServerWindow::setButtonSendEnabled);
}

ServerWindow::~ServerWindow()
{
    delete ui;
}


void ServerWindow::on_button_startServer_clicked()
{
    server.initServer(ui->lineEdit_ipAddress->text(), ui->lineEdit_port->text().toInt());
    ui->button_stopServer->setEnabled(1);
    ui->lineEdit_ipAddress->setEnabled(0);
    ui->lineEdit_port->setEnabled(0);
}

void ServerWindow::on_button_stopServer_clicked()
{
    server.onStopingClicked();
    ui->lineEdit_ipAddress->setEnabled(1);
    ui->lineEdit_port->setEnabled(1);
}

void ServerWindow::updateTextBrowser_log(QString message)
{
    ui->textBrowser_log->append(message);
}

void ServerWindow::updateTextBrowser_messages(QStringList messages)
{
    ui->textBrowser_messages->clear();
    for(int i = 0; i < messages.length(); i++)
        ui->textBrowser_messages->append(messages[i]);
}

void ServerWindow::updateListActiveClients(QStringList arg_Clients)
{
    for (int i = 0; i < arg_Clients.length(); i++)
        if (arg_Clients[i] == "")
            arg_Clients.removeAt(i);   //для удаление пустого элемента в конце списка

    if (loadListOfClient == 0)
    {
        ui->listWidget_connections->addItems(arg_Clients);
        currentListOfClients = arg_Clients;
        loadListOfClient = 1;
    }
    else
    {
        QString changedClient;

        if (arg_Clients.length() > currentListOfClients.length())
        {
            for(int i = 0; i < arg_Clients.length(); i++)
            {
                if (!currentListOfClients.contains(arg_Clients[i]))
                    changedClient = arg_Clients[i];
            }

            QListWidgetItem *item = new QListWidgetItem(changedClient);
            ui->listWidget_connections->addItem(item);
            currentListOfClients.append(changedClient);
        }
        else
        {
            for(int i = 0; i < currentListOfClients.length(); i++)
            {
                if (!arg_Clients.contains(currentListOfClients[i]))
                    changedClient = currentListOfClients[i];
            }

            QString stringOnGroupBox = "Messages of " + changedClient;     //проверка на то, что удаляемый клинт не тот, с котором ведется текущий диалог
            QString currenttextOnGroupBox = ui->groupBox_3->title();
            if (stringOnGroupBox == currenttextOnGroupBox)
            {
                ui->groupBox_3->setTitle("Messages of ");                   //если удаляется текущий клиент, то имя груп бокса сбрасывается на дефорлтное
                ui->button_sendMsg->setEnabled(0);
            }

            QList<QListWidgetItem*> out;
            out = ui->listWidget_connections->findItems(changedClient, Qt::MatchExactly);
            foreach(QListWidgetItem* item, out){
                ui->listWidget_connections->removeItemWidget(item);
                delete item;                                                // Qt documentation warnings you to destroy item to effectively remove it from QListWidget.
            }

            for (int i = 0; i < currentListOfClients.length(); i++)
                if (currentListOfClients[i] == changedClient)
                    currentListOfClients.removeAt(i);
        }
    }
}

void ServerWindow::changeCurrentClient()
{
    ui->button_sendMsg->setEnabled(1);
    server.selectCurrentClient(ui->listWidget_connections->currentItem()->text());
    ui->groupBox_3->setTitle("Messages of " + ui->listWidget_connections->currentItem()->text());
}

void ServerWindow::checkIpAndPort()
{
    if ((ui->lineEdit_ipAddress->text() == "") || (ui->lineEdit_port->text() == ""))
    {
        ui->button_startServer->setEnabled(0);
    }
    else
    {
        ui->button_startServer->setEnabled(1);
    }
}

void ServerWindow::setButtonSendEnabled(int state)
{
    if ((state == Qt::Unchecked) && (server.getNameCurrentClient() == ""))
    {
        ui->button_sendMsg->setEnabled(0);
    }
    else
    {
        ui->button_sendMsg->setEnabled(1);
    }
}

void ServerWindow::on_button_sendMsg_clicked()
{
    if(ui->checkBox->isChecked())
    {
        server.getMessageFromMainWindow(ui->textEdit_message->toPlainText(), ui->checkBox->isChecked());
        ui->textBrowser_messages->append("Server to all: " + ui->textEdit_message->toPlainText());
        ui->textEdit_message->clear();
    }
    else
    {
        if (server.getNameCurrentClient() != "")
        {
            server.getMessageFromMainWindow(ui->textEdit_message->toPlainText(), ui->checkBox->isChecked());
            ui->textBrowser_messages->append("Server to " + server.getNameCurrentClient() + ": " + ui->textEdit_message->toPlainText());
            ui->textEdit_message->clear();
        }
    }
}
