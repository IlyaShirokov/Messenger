#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QDataStream>

class Message : public QObject
{
    Q_OBJECT
public:
    Message();
    Message(int type, QString data);

    friend QDataStream operator <<(QDataStream &out, const Message &msg);
    friend QDataStream operator >>(QDataStream &in, Message &msg);

private:
    int m_type;
    QString m_data;
};

#endif // MESSAGE_H
