#include "Node.hh"

//an empty default constructor
Node::Node()
{
    ;
}

Node::Node(QString hostName, quint16 port)
{
    this->hostName = hostName;
    this->port = port;
    this->address = *(new QHostAddress());
}


Node::Node(QHostAddress address, quint16 port)
{
    this->hostName = "";
    this->address = address;
    this->port = port;
}

