#include "message.h"

Message::Message(QObject *parent) : QObject(parent){}

Message::Message(QString textData, qint8 typeMessage, QString senderName, QString destinationName)
    :m_textData(textData), m_typeMessage(typeMessage), m_senderName(senderName), m_destinationName(destinationName){}


QDataStream& operator<<(QDataStream& out, const Message &msg)
{
    return out << msg.m_typeMessage << msg.m_senderName << msg.m_destinationName << msg.m_textData;
}

QDataStream& operator>>(QDataStream &in, Message &msg)
{
    in >> msg.m_typeMessage;
    in >> msg.m_senderName;
    in >> msg.m_destinationName;
    in >> msg.m_textData;
    return in;
}

QString Message::getTextData()
{
    return m_textData;
}

qint8 Message::getTypeMessage()
{
    return m_typeMessage;
}

QString Message::getSenderName()
{
    return m_senderName;
}

QString Message::getDestinationName()
{
    return m_destinationName;
}
