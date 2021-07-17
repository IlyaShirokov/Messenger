#include "connectdb.h"

ConnectDB::ConnectDB(QObject *parent) : QObject(parent){}

bool ConnectDB::connectDB_server()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("C:\\sqlite\\messenger.db");
    if(!m_db.open())
    {
        qDebug() << "Cannot open database: " << m_db.lastError();
        return false;
    }
    return true;
}

bool ConnectDB::authorizeUser_server(QString name, QString password)
{
    QSqlQuery query;
    QSqlRecord rec;
    QString db_input = "SELECT * FROM users;";

    if(!query.exec(db_input))
    {
        qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
        return false;
    }
    rec = query.record();
    while (query.next())
    {
        QString user_name = query.value(rec.indexOf("name")).toString();
        QString pass = query.value(rec.indexOf("password")).toString();
        if (user_name == name && password == pass)
            return true;
    }
    return false;
}

bool ConnectDB::registerUser_server(QString name, QString password)
{
    if (authorizeUser_server(name, password))
    {
        return false;
    }
    else
    {
        QSqlQuery query;
        QString str_insert = "INSERT INTO users (name, password) VALUES (\"%1\", \"%2\");";
        QString str = str_insert.arg(name).arg(password);
        if(query.exec(str))
        {
            return true;
        }
    }
    return false;
}

void ConnectDB::writeMessageToDB(QString message, QString sender, QString destination)
{
    QSqlQuery query;
    QString str_insert = "INSERT INTO messages (to_userName, from_userName, content) VALUES (\"%1\", \"%2\", \"%3\");";
    QString str = str_insert.arg(destination).arg(sender).arg(message);
    query.exec(str);
}

void ConnectDB::readMessageFromDB_server(QStringList &output, QString sender)
{
    QSqlQuery query;
    QSqlRecord rec;
    QString str_t = "SELECT * FROM messages WHERE to_userName = \"%1\" OR from_userName = \"%1\";";
    QString db_input = str_t.arg(sender);

    query.exec(db_input);
    rec = query.record();
    QString message = "";
    QString userDestination = "";
    QString userSender = "";
    while (query.next())
    {
        message = query.value(rec.indexOf("content")).toString();
        userDestination = query.value(rec.indexOf("to_userName")).toString();
        userSender = query.value(rec.indexOf("from_userName")).toString();
        QString sumString = message + ";" + userDestination + ";" + userSender;
        output.append(sumString);
    }
}

bool ConnectDB::connectDB_client(QString nameClient)
{
    if (!QDir("C:\\UsersDB\\" + nameClient).exists())
    {
        QDir dir;
        dir.mkpath("C:\\UsersDB\\" + nameClient);

        QFile file("C:\\UsersDB\\" + nameClient + "\\" + nameClient + ".db");
        file.open(QIODevice::ReadWrite);
        file.close();

        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName("C:\\UsersDB\\" + nameClient + "\\" + nameClient + ".db");
        if(!m_db.open())
        {
            qDebug() << "Cannot open database: " << m_db.lastError();
            return false;
        }
        else
        {
            QSqlQuery a_query;
            QString str = "CREATE TABLE messages (id INTEGER PRIMARY KEY AUTOINCREMENT, to_userName VARCHAR(100), from_userName VARCHAR(100), content VARCHAR(65000));";
            if (!a_query.exec(str))
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName("C:\\UsersDB\\" + nameClient + "\\" + nameClient + ".db");
        if(!m_db.open())
        {
            qDebug() << "Cannot open database: " << m_db.lastError();
            return false;
        }
        return true;
    }
}

void ConnectDB::readMessageFromDB_client(QStringList &output)
{
    QSqlQuery query;
    QSqlRecord rec;
    QString db_input = "SELECT * FROM messages;";

    query.exec(db_input);
    rec = query.record();
    QString message = "";
    QString userDestination = "";
    QString userSender = "";
    while (query.next())
    {
        message = query.value(rec.indexOf("content")).toString();
        userDestination = query.value(rec.indexOf("to_userName")).toString();
        userSender = query.value(rec.indexOf("from_userName")).toString();
        QString sumString = message + ";" + userDestination + ";" + userSender;
        output.append(sumString);
    }
}
