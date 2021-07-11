#include "clientwindow.h"
#include "ui_clientwindow.h"

ClientWindow::ClientWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientWindow)
{
    ui->setupUi(this);
    ui->button_connectToServer->setEnabled(0);
    loadListOfClient = 0;

    connect(&client, &ClientSocket::updateStatus, this, &ClientWindow::updateTextBrowser); //для записи в текстбраузер из другого класса
    connect(ui->lineEdit_Name, &QLineEdit::textEdited, this, &ClientWindow::checkName); //для проверки на непустоe имя
    connect(&client, &ClientSocket::updateClients, this, &ClientWindow::addItemsToList); //для заполнения листа активных клиентов
    connect(ui->listWidget, &QListWidget::doubleClicked, this, &ClientWindow::changeTitleGroupBox);
}

ClientWindow::~ClientWindow()
{
    delete ui;
}

void ClientWindow::on_button_connectToServer_clicked()
{
    client.setName(ui->lineEdit_Name->text());
    client.newConnect();
    ui->lineEdit_Name->setEnabled(0); //для того чтобы нельзя было менять имя после подключения
}

void ClientWindow::on_button_sendMessage_clicked()
{
    client.sendMessage("1" + ui->textEdit->toPlainText());
    if (client.getNameClientWithCurrentDialog() == "")
        ui->textBrowser->append("Message to server: " + ui->textEdit->toPlainText());
    else
        ui->textBrowser->append("Message to " + client.getNameClientWithCurrentDialog() + ": " + ui->textEdit->toPlainText());
    ui->textEdit->clear();
}

void ClientWindow::updateTextBrowser(QString msg)
{
    if (ui->groupBox->title() == "Dialog with server")
        ui->textBrowser->append("Responce from server: " + msg);
    else
        ui->textBrowser->append("Responce from " + client.getNameClientWithCurrentDialog() + msg);
}

void ClientWindow::checkName()
{
    if (ui->lineEdit_Name->text() != "")
    {
        ui->button_connectToServer->setEnabled(1);
    }
}

void ClientWindow::addItemsToList(QStringList arg_data)
{
    for (int i = 0; i < arg_data.length(); i++)
        if (arg_data[i] == "")
            arg_data.removeAt(i); //для удаление пустого элемента в конце списка

    if (loadListOfClient == 0)
    {
        ui->listWidget->addItems(arg_data);
        currentListOfClients = arg_data;
        loadListOfClient = 1;
    }
    else
    {
        QString changedClient;

        if (arg_data.length() > currentListOfClients.length())
        {
            for(int i = 0; i < arg_data.length(); i++)
            {
                if (!currentListOfClients.contains(arg_data[i]))
                    changedClient = arg_data[i];
            }

            QListWidgetItem *item = new QListWidgetItem(changedClient);
            ui->listWidget->addItem(item);
            currentListOfClients.append(changedClient);
        }
        else
        {
            for(int i = 0; i < currentListOfClients.length(); i++)
            {
                if (!arg_data.contains(currentListOfClients[i]))
                    changedClient = currentListOfClients[i];
            }

            QString stringOnGroupBox = "Dialog with: " + changedClient; //проверка на то, что удаляемый клинт не тот, с котором ведется текущий диалог
            QString currenttextOnGroupBox = ui->groupBox_2->title(); //если удаляется текущий клиент, то имя груп бокса сбрасывается на дефорлтное
            if (stringOnGroupBox == currenttextOnGroupBox)
                ui->groupBox_2->setTitle("Dialog with server");

            QList<QListWidgetItem*> out;
            out = ui->listWidget->findItems(changedClient, Qt::MatchExactly);
            foreach(QListWidgetItem* item, out){
                ui->listWidget->removeItemWidget(item);
                delete item; // Qt documentation warnings you to destroy item to effectively remove it from QListWidget.
            }

            for (int i = 0; i < currentListOfClients.length(); i++)
                if (currentListOfClients[i] == changedClient)
                    currentListOfClients.removeAt(i);
        }
    }
}

void ClientWindow::changeTitleGroupBox()
{
    if (ui->listWidget->currentItem()->text() == client.getName())
        return;
    ui->groupBox_2->setTitle("Dialog with: " + ui->listWidget->currentItem()->text());

    client.setNameClientWithCurrentDialog(ui->listWidget->currentItem()->text());
    client.selectUserForDialog(ui->listWidget->currentItem()->text());
}
