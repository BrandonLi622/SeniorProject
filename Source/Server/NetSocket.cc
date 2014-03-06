#include "NetSocket.hh"

NetSocket::NetSocket(quint32 numC, quint32 numS)
{
    // Pick a range of four UDP ports to try to allocate by default,
    // computed based on my Unix user ID.
    // This makes it trivial for up to four Peerster instances per user
    // to find each other on the same host,
    // barring UDP port conflicts with other applications
    // (which are quite possible).
    // We use the range from 32768 to 49151 for this purpose.

    this->numClients = numC;
    this->numServers = numS;

    myPortMin = 32768 + (getuid() % 4096)*4 + numClients;
    myPortMax = myPortMin + numServers - 1;

    viewNum = 0;
    serverIDs = calcViewMembership(viewNum);

    allClients = new QList<Node *>();
    directClients = new QList<Node *>();
    allServers = new QList<Node *>();
    clientWait = new QList<quint32>();
    serverWait = new QList<quint32>();
    idLookup = new QHash<quint16, quint32>();
    receivedClientMsgs = new QHash<quint32, QByteArray>();
    receivedServerCiphers = new QHash<quint32, QByteArray>();
    viewChangeProposals = new QHash<quint32, QVariantMap>();
}


void NetSocket::connectSignals()
{
    this->connect(this, SIGNAL(readyRead()), this, SLOT(gotMessage()));
    this->connect(chat, SIGNAL(sendViewVote(quint32)), this, SLOT(proposeViewChange(quint32)));
}

void NetSocket::init()
{

    //keep track of all of the servers
    for (int i = 0; i < numServers; i++)
    {
        Node *server = new Node();
        server->address = QHostAddress(QHostAddress::LocalHost);
        server->port = myPortMin + i;
        allServers->append(server);
        idLookup->insert(server->port, i);
    }

    //keep track of all of the clients
    for (int i = 0; i < numClients; i++)
    {
        Node *client = new Node();
        client->address = QHostAddress(QHostAddress::LocalHost);
        client->port = myPortMin - numClients + i;
        this->allClients->append(client);
    }

    assignClients();
    startRound();
}

void NetSocket::startRound()
{
    roundPhase = 0;
    receivedClientMsgs->clear();
    receivedServerCiphers->clear();

    //At this point clientWait should be empty
    //Waiting for all clients
    clientWait->clear();
    for (int i = 0; i < directClients->length(); i++)
    {
        Node *client = directClients->value(i);
        quint32 clientID = idLookup->value(client->port);
        clientWait->append(clientID);
    }

    //Need to receive all of the server ciphertexts
    serverWait->clear();
    for (int i = 0; i < serverIDs->length(); i++)
    {
        serverWait->append(serverIDs->value(i));
    }
}

//Should theoretically not start this until client messages are all collected
void NetSocket::startCipherCollect()
{
    roundPhase = 1;
}

//TODO: This assumes all servers are online
void NetSocket::assignClients()
{
    this->directClients->clear();

    for (int i = 0; i < numClients; i++)
    {
        int indexInView = serverIDs->indexOf(thisID);
        if (indexInView == i % serverIDs->length())
        {
            this->directClients->append(allClients->value(i));
        }

        idLookup->insert(myPortMin - numClients + i, i); //I'm assuming this is true...
    }

    qDebug() << "Direct Clients";
    qDebug() << directClients->length();
    qDebug() << serverIDs->length();

}

void NetSocket::changeView(quint32 newView)
{
    this->viewNum = newView;
    serverIDs = calcViewMembership(viewNum); //Must happen before assignClients because the serverIDs must be updated
    assignClients();

    QVariantMap map;
    map.insert("NewViewNum", newView);

    for (int i = 0; i < directClients->length(); i++)
    {
        Node *client = directClients->value(i);
        sendMessage(client, map);
    }
}

//Same as in client, theoretically
QList<quint32> *NetSocket::calcViewMembership(quint32 viewNum)
{
    QList<quint32> *servers = new QList<quint32>();

    srand(viewNum); //Seed the PRNG with the view number

    while (servers->length() < 2.0 * numServers / 3.0)
    {
        quint32 nextServ = (quint32) rand() % numServers;
        if (!servers->contains(nextServ))
        {
            servers->append(nextServ);
        }
    }

    qDebug() << *servers;

    return servers;
}



QByteArray NetSocket::encryptionKey(quint32 serverID, quint32 clientID)
{
    QByteArray key;

    srand(serverID * 100 + clientID);
    for (int i = 0; i < numClients; i++)
    {
        for (int j = 0; j < chat->slotLength; j++)
        {
            char num = char(32 + rand() % 96); //Just for testing purposes make them all printable characters
            key.append(num); // Pick a random character
            qDebug() << "chosen number " << num;

        }
    }

    qDebug() << serverID << " " << clientID;
    qDebug() << key << key.length();
    return key;
}

//Utility
QByteArray xorBytes(QByteArray a1, QByteArray a2)
{
    //Assuming strings are the same length
    QByteArray xored;

    if (a1.length() != a2.length())
    {
        qDebug() << "Warning: xoring 2 bytearrays of different lengths";
    }

    for (int i = 0; i < a1.length(); i++)
    {
        xored.append(a1.at(i) ^ a2.at(i));
    }
    return xored;
}

//Pass in the client's combined text
//Should only call this if we have at least one client, am I right?
QByteArray NetSocket::createCipher()
{
    qDebug() << "All messages";
    qDebug() << *receivedClientMsgs;

    //Xor's the bits from the client first
    quint32 id = receivedClientMsgs->keys().value(0);
    QByteArray xored = receivedClientMsgs->value(id);
    for (int i = 1; i < receivedClientMsgs->keys().length(); i++)
    {
        id = receivedClientMsgs->keys().value(i);
        xored = xorBytes(xored, receivedClientMsgs->value(id));
    }

    //Assuming fixed length messages
    //Gets the encryption key for all of the clients
    for (int i = 0; i < allClients->count(); i++)
    {
        Node *client = allClients->value(i);
        quint32 port = client->port;
        quint32 clientID = idLookup->value(port);

        QByteArray key = encryptionKey(thisID, clientID);
        if (key == "")
        {
            return "";
        }
        xored = xorBytes(xored, key);
    }
    return xored;
}

bool NetSocket::bind()
{
    // Try to bind to each of the range myPortMin..myPortMax in turn.
    for (int p = myPortMin; p <= myPortMax; p++) {
        if (QUdpSocket::bind(p)) {
            qDebug() << "bound to UDP port " << p;
            thisID = p - myPortMin;  //Just to index the servers...
            thisPort = p;
            thisNode = new Node(QHostAddress(QHostAddress::LocalHost), p);
            this->init();


            return true;
        }
    }

    qDebug() << "Oops, no ports in my default range " << myPortMin
        << "-" << myPortMax << " available";
    return false;
}

void NetSocket::sendMessage(Node *neighbor, QVariantMap message)
{
    //if it's not a valid address, don't try sending it to anyone
    //a possible case could be that this was called from propagatePrivate and it couldn't find a route
    if (neighbor->address.isNull())
    {
        qDebug() << "no neighbor...";
        return;
    }

    qDebug() << "Sent to: " << neighbor->address << QString::number(neighbor->port);
    qDebug() << message;

    QByteArray serialized;
    QDataStream *stream = new QDataStream(&serialized, QIODevice::WriteOnly);
    (*stream) << message;

    writeDatagram(serialized, neighbor->address, neighbor->port);

    //Just debugging output
    //QString test = "From " + QString::number(this->getThisPort()) + " to " + QString::number(neighbor.port);
}

QVariantMap NetSocket::deserializeMessage(QHostAddress *messageSource, quint16 *messageSourcePort)
{
    QByteArray datagram;
    datagram.resize(pendingDatagramSize());
    readDatagram(datagram.data(), datagram.size(), messageSource, messageSourcePort);

    QDataStream stream(&datagram, QIODevice::ReadOnly);
    QVariantMap map;
    stream >> map;

    return map;
}


//WORKS
void NetSocket::gotMessage()
{

    QHostAddress *messageSource = new QHostAddress();
    quint16 *messageSourcePort = new quint16();
    QVariantMap map = deserializeMessage(messageSource, messageSourcePort);
    switchMessage(map, messageSource, messageSourcePort);

    qDebug() << map;

}

//Change this!
void NetSocket::switchMessage(QVariantMap map, QHostAddress *messageSource, quint16 *messageSourcePort)
{
    if (map.keys().contains("ViewNum") && map.keys().contains("ClientID") && map.keys().contains("Message"))
    {
        processMessage(map);

        //proposeViewChange(1); //Just for testing
    }


    //Do we want to include round number here?
    else if (map.keys().contains("Cipher") && map.keys().contains("ServerID"))
    {
        processCipher(map);
    }

    else if (map.keys().contains("NewProposedView"))
    {
        processViewChangeProposal(map, messageSource, messageSourcePort);
    }


    else
    {
        //IT SHOULD NEVER REACH HERE. If it gets here then we don't do anything
        qDebug() << "Malformed message";
    }
}

void NetSocket::processCipher(QVariantMap map)
{
    quint32 serverID = map.value("ServerID").toUInt();
    QByteArray cipher = map.value("Cipher").toByteArray();
    receivedServerCiphers->insert(serverID, cipher);
    serverWait->removeOne(serverID);

    qDebug() << "Received cipher";

    if (serverWait->empty() && roundPhase == 1 && serverIDs->contains(thisID))
    {
        sendDecrypted();
    }
}

void NetSocket::sendDecrypted()
{
    QList<QByteArray> allCiphers = receivedServerCiphers->values();
    QByteArray decrypted = allCiphers.value(0);

    for (int i = 1; i < allCiphers.length(); i++)
    {
        decrypted = xorBytes(decrypted, allCiphers.value(i));
    }

    QVariantMap map;
    map.insert("ReturnMessage", decrypted);
    map.insert("ViewNum", viewNum);

    //Broadcast results
    for (int i = 0; i < directClients->length(); i++)
    {
        sendMessage(directClients->value(i), map);
    }

    //End of the round!
    qDebug() << decrypted;
    startRound();
}

void NetSocket::processMessage(QVariantMap map)
{
    if (roundPhase != 0)
    {
        qDebug() << "Cannot collect messages anymore";
        return;
    }

    quint32 viewNum = map.value("ViewNum").toUInt();
    quint32 clientID = map.value("ClientID").toUInt();
    QByteArray msg = map.value("Message").toByteArray();

    //Don't do anything about wrong viewNum's

    receivedClientMsgs->insert(clientID, msg);
    clientWait->removeOne(clientID);

    emit this->receivedMessage(msg, clientID, viewNum);
    qDebug() << "Success!";

    if (clientWait->empty())
    {
        startCipherCollect();
        broadcastCiphers();
    }
}

void NetSocket::processViewChangeProposal(QVariantMap map, QHostAddress *messageSource, quint16 *messageSourcePort)
{
    //Also make sure not to double count per server!!! (how do I do that?)
    quint32 viewNum = map.value("NewProposedView").toUInt();
    QVariantMap countMap = viewChangeProposals->value(viewNum, QVariantMap()); //want it to default to 0

    quint32 count = countMap.value("Count", 0).toUInt();
    QList<QVariant> voters = countMap.value("Voters", QList<QVariant>()).toList();

    quint32 serverID = idLookup->value(*messageSourcePort);

    qDebug() << "Attempted view change " << *messageSourcePort << " " << serverID;
    qDebug() << *idLookup;

    //Don't allow double counting
    if (voters.contains(serverID))
    {
        qDebug() << "Redundant voters " << voters;
        return;
    }

    voters.append(serverID);
    countMap.insert("Count", count + 1);
    countMap.insert("Voters", voters);

    viewChangeProposals->insert(viewNum, countMap);

    qDebug() << "COUNT: " << (count + 1);

    //TODO: should move out this proportion somewhere else...
    if (count + 1 >= 2.0 * numServers / 3.0 && viewNum > this->viewNum)
    {
        qDebug() << "Changed view";
        changeView(viewNum);
        emit this->displayViewChange(viewNum, serverIDs->contains(thisID));
        startRound(); //Need to discard everything and start a new round when the view changes
    }
}

void NetSocket::proposeViewChange(quint32 newView)
{
    QVariantMap map;
    map.insert("NewProposedView", newView);

    for (int i = 0; i < allServers->length(); i++)
    {
        Node *server = allServers->value(i);
        sendMessage(server, map);
    }

    //sendMessage(thisNode, map); //Should consider own vote!


}

void NetSocket::broadcastCiphers()
{
    if (receivedClientMsgs->keys().length() == 0)
    {
        return;
    }

    QByteArray thisCipher = createCipher();

    //Don't really need these anymore
    //serverWait->removeOne(thisID);
    //receivedServerCiphers->insert(thisID, thisCipher);

    qDebug() << "My cipher";
    qDebug() << thisCipher;

    for (int i = 0; i < serverIDs->length(); i++)
    {
        Node *server = allServers->value(serverIDs->value(i));
        QVariantMap map;
        map.insert("Cipher", thisCipher);
        map.insert("ServerID", thisID);
        sendMessage(server, map);
    }

    if (serverWait->empty() && roundPhase == 1)
    {
        sendDecrypted();
    }
}
