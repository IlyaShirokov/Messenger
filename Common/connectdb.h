#ifndef CONNECTDB_H
#define CONNECTDB_H

#include <QObject>
#include <QtSql>
#include <QDir>

class ConnectDB : public QObject
{
    Q_OBJECT
public:
    explicit ConnectDB(QObject *parent = nullptr);
    bool connectDB_server();
    bool authorizeUser_server(QString name, QString password);
    bool registerUser_server(QString name, QString password);
    void writeMessageToDB(QString message, QString sender, QString destination);
    void readMessageFromDB_server(QStringList &output, QString sender);

    bool connectDB_client(QString nameClient);
    void readMessageFromDB_client(QStringList &output);

signals:

private:
    QSqlDatabase m_db;  // экземпляр подключения к БД

};

#endif // CONNECTDB_H
