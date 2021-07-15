#ifndef CONNECTDB_H
#define CONNECTDB_H

#include <QObject>

class ConnectDB : public QObject
{
    Q_OBJECT
public:
    explicit ConnectDB(QObject *parent = nullptr);

signals:

};

#endif // CONNECTDB_H
