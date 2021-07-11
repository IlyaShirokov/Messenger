#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include "clientsocket.h"

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
    void on_button_connectToServer_clicked();
    void on_button_sendMessage_clicked();
    void updateTextBrowser(QString msg);     //слод для обнолвения информации в nextBrowser
    void checkName(); //для проверки непустой строки в имени
    void addItemsToList(QStringList arg_data);
    void changeTitleGroupBox();

private:
    Ui::ClientWindow *ui;
    ClientSocket client;
    QStringList currentListOfClients;
    bool loadListOfClient; //становится единицей при первой загрузке списка клиентов при подключении
};
#endif // CLIENTWINDOW_H
