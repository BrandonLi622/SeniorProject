#include "main.hh"

/*
 * Questions and stuff
 *
 * I am assuming no malicious nodes
 * Do I put null characters where there are other people's messages?
 *
 * So the servers will decide the views (which the clients can individually calculate). The clients don't care about who is offline, they assume the servers correctly figured that out.
 *
 *
 *
 *
 *
 *
 *
 *
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

    dialog.setPosition(dialog.width() * sock.thisID,500);

    dialog.setWindowTitle("Dissent Server : Port " + QString::number(sock.thisPort));
    dialog.connectSignals();
    sock.connectSignals();


    srand (0);
    //qDebug() << rand();

    // Enter the Qt main loop; everything else is event driven
	return app.exec();
}

