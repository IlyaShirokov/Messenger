#ifndef CONNECTDB_H
#define CONNECTDB_H

#include <QObject>
#include <QtSql>

class ConnectDB : public QObject
{
    Q_OBJECT
public:
    explicit ConnectDB(QObject *parent = nullptr);
    bool connectDB();   //подключение к БД
    bool authorizeUser(QString name, QString password);
    bool registerUser(QString name, QString password);
    void writeMessageToDB(QString message, QString sender, QString destination);
    void readMessageFromDB(QStringList &output, QString sender);

signals:

private:
    QSqlDatabase m_db;  // экземпляр подключения к БД

};

#endif // CONNECTDB_H
