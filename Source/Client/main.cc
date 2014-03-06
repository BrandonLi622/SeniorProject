#include "main.hh"

/*
 * Questions and stuff
 *
 * I am assuming no malicious nodes
 * Do I put null characters where there are other people's messages?
 *
 * So the servers will decide the views (which the clients can individually calculate). The clients don't care about who is offline, they assume the servers correctly figured that out.
 * I skip the hashing here and go straight to collecting texts
 * I have a timer for how long until a server is considered offline
 * I assume every client participates in every round (what if their server is not in the round?..)
 *
 * Types of messages:
 * - Client sends a message to a server               (("ViewNum", quint32), ("ClientID", quint32), ("Message", QByteArray))
 * - Server sends a view change to a client           (("NewViewNum", quint32))
 * - Server sends the resulting text to the client    (("ReturnMessage", QByteArray))
 * - Server sends a ciphertext to another server      (("Cipher", QByteArray), ("ServerID", quint32))
 * - Server proposes a view change to another server  (("NewProposedView", quint32))
 *
 *
 * View changes are complicated... do I want a proposer or everyone voting in a free for all?
 * Who does a client connect to in a particular view?
 * Right now my thing only collects messages when it is not waiting for responses
 * It allows people to send multiple messages per round, I think
 * BUG: Everything goes to uppercase for some reason!
 *
 * Learned that encoding is really a problem
 * Wowww so .replace is an in place operation
 *
 *
 *
 * View change in middle of round causes wrong to restart
 *Right now you can send multiple messages in a round and they will just use the last message you sent before the time expired
 *IDEA: Maybe can get view change as a majority vote that is broadcasted to clients?
 *
 */

int main(int argc, char **argv)
{

    quint32 numServs = 3;
    quint32 numClis = 6;
    quint32 slotLength = 1;


    if (argc >= 4)
    {
        numServs = atoi(argv[1]);
        numClis = atoi(argv[2]);
        slotLength = atoi(argv[3]);

        if (numServs <= 0 || numClis <= 0 || slotLength <= 0)
        {
            qDebug() << "Call as ./Server {numServers} {numClients} {slotLength} where all args are positive integers";
            exit(0);
        }
    }
    else
    {
        qDebug() << "Call as ./Server {numServers} {numClients} {slotLength} where all args are positive integers";
        qDebug() << "Using default number of clients and servers (6,3)";
    }

    // Initialize Qt toolkit
    QApplication app(argc,argv);

    // Create an initial chat dialog window
    ChatDialog dialog(slotLength);
    dialog.show();

    // Create a UDP network socket
    NetSocket sock(numClis, numServs);
    if (!sock.bind())
        exit(1);

    dialog.sock = &sock;
    sock.chat = &dialog;

    dialog.setPosition(dialog.width() * sock.thisID,0);


    dialog.setWindowTitle("Dissent Client : Port " + QString::number(sock.thisPort));


    sock.connectSignals();
    dialog.connectSignals();

    srand (0);
    //qDebug() << rand();

    // Enter the Qt main loop; everything else is event driven
	return app.exec();
}

