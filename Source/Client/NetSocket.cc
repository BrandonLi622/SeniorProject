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


    myPortMin = 32768 + (getuid() % 4096)*4;
    myPortMax = myPortMin + numClients - 1;

    viewNum = 0;
    serverIDs = calcViewMembership(viewNum);
    allServers = new QList<Node *>();
    connectedServer = new Node();
    findAllServers();
}

void NetSocket::findAllServers()
{
    for (int i = 0; i < numServers; i++)
    {
        Node *server = new Node(QHostAddress(QHostAddress::LocalHost), myPortMax + i + 1);
        allServers->append(server);
    }
}

QList<quint32> *NetSocket::calcViewMembership(quint32 viewNum)
{
    QList<quint32> *servers = new QList<quint32>();

    srand(viewNum); //Seed the PRNG with the view number

    //TODO: Need to make it a fraction
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

void NetSocket::connectSignals()
{
    this->connect(chat, SIGNAL(servSelected(QHostAddress,quint16)), this, SLOT(connectToServer(QHostAddress,quint16)));
    this->connect(chat, SIGNAL(msgInputted(QString)), this, SLOT(sendRoundMessage(QString)));
    this->connect(this, SIGNAL(readyRead()), this, SLOT(gotMessage()));
}

void NetSocket::connectToServer(QHostAddress address, quint16 port)
{
    connectedServer->address = address;
    connectedServer->port = port;

    qDebug() << "Connected to server at: " << address << ":" << QString::number(port);
}

//A round message consists of ("ViewNum", quint32) ("ClientID", quint32) and ("Message", QByteArray)
void NetSocket::sendRoundMessage(QString msg)
{
    QByteArray encrypted = encryptMessage(insertInSlot(msg));

    //QByteArray decrypted = encryptMessage(encrypted);
    //decrypted = decrypted.replace('\0', ' '); //for the sake of printing, since qDebug does not like null characters

    qDebug() << encrypted;

    if (encrypted == "")
    {
        return;
    }

    QVariantMap *map = new QVariantMap();
    map->insert("ViewNum", this->viewNum);
    map->insert("ClientID", this->thisID);
    map->insert("Message", encrypted);

    sendMessage(this->connectedServer, *map);
}

QByteArray xorBytes(QByteArray a1, QByteArray a2)
{
    //Assuming strings are the same length
    QByteArray xored;

    if (a1.length() != a2.length())
    {
        qDebug() << "Warning: xoring 2 bytearrays of different lengths";
        qDebug() << a1.length() << " " << a1;
        qDebug() << a2.length() << " " << a2;
    }

    for (int i = 0; i < a1.length(); i++)
    {
        qDebug() << "xoring";
        qDebug() << quint32(a1.at(i)) << "   " << quint32(a2.at(i));
        qDebug() << quint32(a1.at(i) ^ a2.at(i));

        xored.append(a1.at(i) ^ a2.at(i));
    }
    return xored;
}

QByteArray NetSocket::insertInSlot(QString msg)
{
    //So far message only contains my own slot
    QByteArray fullMsg;
    for (int i = 0; i < slotNum; i++)
    {
        //null values for other people's
        for (int j = 0; j < chat->slotLength; j++)
        {
            fullMsg.append(char(0));
        }
    }

    fullMsg.append(msg); //Add in my message;

    //Add in the other crap
    for (int i = slotNum + 1; i < numClients; i++)
    {
        //null values for other people's
        for (int j = 0; j < chat->slotLength; j++)
        {
            fullMsg.append(char(0));
        }
    }

    QByteArray testMsg;
    testMsg.append(fullMsg);
    qDebug() << "Full msg: " << testMsg.replace('\0', ' ');

    //qDebug() << "Full msg: " << fullMsg.replace('\0', ' ');

    return fullMsg;
}


//Should this be returning a string or a byte array?
QByteArray NetSocket::encryptMessage(QByteArray msg)
{
    qDebug() << "Pre-Encrypted";
    //qDebug() << msg.replace('\0', ' ');

    QByteArray xored = msg;

    //Assuming fixed length messages
    for (int i = 0; i < serverIDs->count(); i++)
    {
        quint32 id = serverIDs->value(i);
        QByteArray key = encryptionKey(id, thisID);
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
            thisID = p - myPortMin;  //Just to index the clients...
            thisPort = p;
            assignServer();
            slotNum = thisID; //TODO: Just a default
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

}

//for deciding what type of message it is and then responding accordingly
void NetSocket::switchMessage(QVariantMap map, QHostAddress *messageSource, quint16 *messageSourcePort)
{
    if (map.keys().contains("NewViewNum"))
    {
        processViewChange(map);
    }


    //Do we want to include round number here?
    //TODO: put the view number here so the clients know
    else if (map.keys().contains("ReturnMessage") && map.keys().contains("ViewNum"))
    {
        processReturnedMessage(map);
    }


    else
    {
        //IT SHOULD NEVER REACH HERE. If it gets here then we don't do anything
        qDebug() << "Malformed message";
    }
}

void NetSocket::processReturnedMessage(QVariantMap map)
{
    QByteArray msg = map.value("ReturnMessage").toByteArray();
    quint32 viewNum = map.value("ViewNum").toUInt();
    emit this->displayReturnedMsg(msg, viewNum);
}

void NetSocket::processViewChange(QVariantMap map)
{
    quint32 viewNum = map.value("NewViewNum").toUInt();
    qDebug() << "View Change: " << viewNum;
    this->viewNum = viewNum;
    this->serverIDs = calcViewMembership(viewNum);
    qDebug() << serverIDs;

    //Need to pick a new server now!!
    assignServer();

    emit this->displayViewChange(viewNum);
}

void NetSocket::assignServer()
{
    //Chooses the index in the view
    quint32 servID = thisID % serverIDs->length();
    //Then connects to the server associated with that index
    Node *server = allServers->value(serverIDs->value(servID));

    qDebug() << "Now connected to port " << server->port;
    connectToServer(server->address, server->port);
}
