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
    void receivedMessage(QByteArray msg, quint32 id, quint32 viewNum);
    void displayViewChange(quint32 viewNum, bool isMember);

public slots:
    //void sendRoundMessage(QString msg);
    //void connectToServer(QHostAddress address, quint16 port);

    //void connectSignals();
    void connectSignals();

    void gotMessage();
    QVariantMap deserializeMessage(QHostAddress *messageSource, quint16 *messageSourcePort);
    void switchMessage(QVariantMap map, QHostAddress *messageSource, quint16 *messageSourcePort);
    void proposeViewChange(quint32 newView);

    //void processViewChange(QVariantMap map);
    //void processReturnedMessage(QVariantMap map);



private:
    int myPortMin, myPortMax;
    //QByteArray encryptMessage(QByteArray msg);
    QByteArray createCipher();
    void sendMessage(Node *neighbor, QVariantMap message);

    quint32 viewNum;
    QList<quint32> *serverIDs; //The servers in the current view, perhaps could use a better name
    Node *thisNode;

    QList<Node *> *directClients;
    QList<Node *> *allClients;                  //Indexed by ID
    QList<Node *> *allServers;                  //Including itself
    QList<quint32> *clientWait;                 //When waiting for messages to be received from clients
    QList<quint32> *serverWait;                 //When waiting for all ciphertexts to be received

    //key is the port number (assume all on local machine for now), value is the ID number
    QHash<quint16, quint32> *idLookup;

    //For both of these, the key is the ID
    QHash<quint32, QByteArray> *receivedClientMsgs;
    QHash<quint32, QByteArray> *receivedServerCiphers;
    //Node *connectedServer;

    //Key is the view number and value is a map with pairs (("Count", quint32), ("Voters", QList<QVariant>))
    //Just know that the QVariant's will always be quint32
    QHash<quint32, QVariantMap> *viewChangeProposals;


    //quint32 slotNum;

    QByteArray encryptionKey(quint32 serverID, quint32 clientID);
    QList<quint32> *calcViewMembership(quint32 viewNum);

    void startRound();      //***
    void startCipherCollect(); //***
    void assignClients();   //***
    void init();

    void processCipher(QVariantMap map);
    void processMessage(QVariantMap map);
    void processViewChangeProposal(QVariantMap map, QHostAddress *messageSource, quint16 *messageSourcePort);
    void broadcastCiphers();
    void sendDecrypted();
    void changeView(quint32 newView);

    int roundPhase; //0 means collecting messages, 1 means collecting ciphertexts

};

#endif // NETSOCKET_HH
