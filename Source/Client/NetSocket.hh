#ifndef NETSOCKET_HH
#define NETSOCKET_HH

#include "CommonIncludes.hh"
#include "ChatDialog.hh"
#include "Node.hh"

class ChatDialog;
class Node;

class NetSocket : public QUdpSocket
{
    Q_OBJECT

public:
    NetSocket(quint32 numC, quint32 numS);
    ChatDialog *chat;
    quint32 thisPort;
    quint32 numClients; //In this scheme numClients has to be < 10 because of the way we are forming the encryption keys
    quint32 numServers;
    quint32 thisID;

    // Bind this socket to a Peerster-specific default port.
    bool bind();

signals:
    void displayReturnedMsg(QByteArray msg, quint32 viewNum);
    void displayViewChange(quint32 viewNum);

public slots:
    void sendRoundMessage(QString msg);
    void connectToServer(QHostAddress address, quint16 port);

    void connectSignals();

    void gotMessage();
    QVariantMap deserializeMessage(QHostAddress *messageSource, quint16 *messageSourcePort);
    void switchMessage(QVariantMap map, QHostAddress *messageSource, quint16 *messageSourcePort);

    void processViewChange(QVariantMap map);
    void processReturnedMessage(QVariantMap map);



private:
    int myPortMin, myPortMax;
    QByteArray encryptMessage(QByteArray msg);
    void sendMessage(Node *neighbor, QVariantMap message);

    quint32 viewNum;
    QList<quint32> *serverIDs;
    Node *connectedServer;

    QList<Node*> *allServers;


    quint32 slotNum;

    QByteArray encryptionKey(quint32 serverID, quint32 clientID);
    QByteArray insertInSlot(QString msg);


    QList<quint32> *calcViewMembership(quint32 viewNum);

    void assignServer();
    void findAllServers();


    /*
     * Alternative encryption
     *     quint32 uniqueID = serverID * 10 + clientID;
    for (int i = 0; i < numClients; i++)
    {
        for (int j = 0; j < chat->slotLength; j++)
        {
            key.append(uniqueID);
        }
    }

    qDebug() << serverID << " " << clientID;
    qDebug() << key << key.length();
    return key;
     *
     *
     *
     *
     */

};

#endif // NETSOCKET_HH
