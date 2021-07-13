#include "message.h"

Message::Message(){}

Message::Message(int type, QString data) : m_type(type), m_data(data){}

QDataStream operator <<(QDataStream &out, const Message &msg)
{

}

QDataStream operator >>(QDataStream &in, Message &msg)
{

}
