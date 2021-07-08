#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include "serversocket.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ServerWindow; }
QT_END_NAMESPACE

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();

private slots:
    void on_button_startServer_clicked();
    void on_button_stopServer_clicked();
    void on_button_send_clicked();
    void updateTextBrowser_log(QString message); //слот для обновления информации о логе сервера

private:
    Ui::ServerWindow *ui;
    ServerSocket server;
    QStringList currentListOfUsers;
};
#endif // SERVERWINDOW_H
