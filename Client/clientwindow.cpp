#include "clientwindow.h"
#include "ui_clientwindow.h"

ClientWindow::ClientWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientWindow)
{
    ui->setupUi(this);
    loadListOfClient = 0;  

    connect(&auth, &Authentication::sendLogin, &client, &ClientSocket::setConnect);
    connect(&auth, &Authentication::sendRegister, &client, &ClientSocket::registerClient);
    connect(&client, &ClientSocket::loginUserAnswerDB, &auth, &Authentication::loginUserAnswerDB);
    connect(&client, &ClientSocket::registerUserAnswerDB, &auth, &Authentication::registerUserAnswerDB);

    connect(&client, &ClientSocket::updateMessages, this, &ClientWindow::updateTextBrowser_messages);               //для записи в текстбраузер из другого класса
    connect(&client, &ClientSocket::updateClients, this, &ClientWindow::changeUsersInList);                         //для заполнения листа активных клиентов
    connect(ui->listWidget_activeClients, &QListWidget::doubleClicked, this, &ClientWindow::changeUserDialogWith);  //выбор клиента для диалога с ним
    connect(&client, &ClientSocket::serverDisconnected, this, &ClientWindow::serverDisconnected);                   //действия при потере соеднинения с сервером
    connect(&client, &ClientSocket::displayClientWindow, this, &ClientWindow::display);                             //сигнал от сокета о успешкном подключении и отображении главного окна

    auth.show();
}

ClientWindow::~ClientWindow()
{
    delete ui;
}

void ClientWindow::on_button_sendMessage_clicked()
{
    if (client.getNameClientWithCurrentDialog() != "")
    {
        client.getMessageFromWindow(ui->textEdit_message->toPlainText());
        ui->textBrowser_dialog->append(client.getName() + " to " + client.getNameClientWithCurrentDialog() + ": " + ui->textEdit_message->toPlainText());
        ui->textEdit_message->clear();
    }
}

void ClientWindow::updateTextBrowser_messages(QString msg)
{
    QStringList structMsg = msg.split(';');
    ui->textBrowser_dialog->append(structMsg[0] + ": " + structMsg[1]);
}

void ClientWindow::changeUsersInList(QStringList arg_data)
{
    for (int i = 0; i < arg_data.length(); i++)
        if (arg_data[i] == "")
            arg_data.removeAt(i);   //для удаление пустого элемента в конце списка

    if (loadListOfClient == 0)
    {
        ui->listWidget_activeClients->addItems(arg_data);
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
            ui->listWidget_activeClients->addItem(item);
            currentListOfClients.append(changedClient);
        }
        else
        {
            for(int i = 0; i < currentListOfClients.length(); i++)
            {
                if (!arg_data.contains(currentListOfClients[i]))
                    changedClient = currentListOfClients[i];
            }

            QString stringOnGroupBox = "Dialog with: " + changedClient;     //проверка на то, что удаляемый клинт не тот, с котором ведется текущий диалог
            QString currenttextOnGroupBox = ui->groupBox_dialog->title();
            if (stringOnGroupBox == currenttextOnGroupBox)                  //если удаляется текущий клиент, то имя груп бокса сбрасывается на дефорлтное
            {
                client.selectUserForDialog("");
                ui->groupBox_dialog->setTitle("Dialog with: ");
            }

            QList<QListWidgetItem*> out;
            out = ui->listWidget_activeClients->findItems(changedClient, Qt::MatchExactly);
            foreach(QListWidgetItem* item, out){
                ui->listWidget_activeClients->removeItemWidget(item);
                delete item;                                                // Qt documentation warnings you to destroy item to effectively remove it from QListWidget.
            }

            for (int i = 0; i < currentListOfClients.length(); i++)
                if (currentListOfClients[i] == changedClient)
                    currentListOfClients.removeAt(i);
        }
    }
}

void ClientWindow::changeUserDialogWith()
{
    if (ui->listWidget_activeClients->currentItem()->text() == client.getName())
        return;
    ui->groupBox_dialog->setTitle("Dialog with: " + ui->listWidget_activeClients->currentItem()->text());

    client.selectUserForDialog(ui->listWidget_activeClients->currentItem()->text());
}

void ClientWindow::serverDisconnected()
{
    ui->button_sendMessage->setEnabled(0);
    ui->listWidget_activeClients->clear();
    currentListOfClients.clear();
    loadListOfClient = 0;
    ui->textBrowser_dialog->append("<nobr><font color=\"red\">Server closed at " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "</nobr>");
}

void ClientWindow::display()
{
    auth.hide();
    this->show();
}
