#ifndef NODE_HH
#define NODE_HH

#include "CommonIncludes.hh"

class Node
{
public:
    Node();
    Node(QHostAddress address, quint16 port);
    Node(QString hostName, quint16 port);

    QString hostName;     //not always filled if the address is directly provided
    QHostAddress address;
    quint16 port;
};

#endif // NODE_HH

