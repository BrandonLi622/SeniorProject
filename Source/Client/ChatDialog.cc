#include "ChatDialog.hh"


QString getHeaderStyle()
{
    return "QLineEdit {  border: 1px solid black; height: 30}";
}

QString getViewLabel(quint32 viewNum)
{
    return "Current view: " + QString::number(viewNum);
}

quint32 startingViewNum()
{
    return 0;
}

ChatDialog::ChatDialog(quint32 slotLength)
{
    this->slotLength = slotLength;

    // Read-only text box where we display messages from everyone.
    // This widget expands both horizontally and vertically.
    textview = new QTextEdit(this);
    textview->setReadOnly(true);

    // Small text-entry box the user can enter messages.
    // This widget normally expands only horizontally,
    // leaving extra vertical space for the textview widget.
    //
    // You might change this into a read/write QTextEdit,
    // so that the user can easily enter multi-line messages.
    textline = new QLineEdit(this);
    textline->setMaxLength(slotLength);


    //All of the headers
    QLineEdit *msgHeader = new QLineEdit();
    msgHeader->setText("Send message here: (Fixed-length " + QString::number(slotLength) + "-char messages only)");
    msgHeader->setFixedWidth(400);
    msgHeader->setReadOnly(true);
    msgHeader->setStyleSheet(getHeaderStyle());

    QLineEdit *servHeader = new QLineEdit();
    servHeader->setText("Select server port on local machine");
    servHeader->setFixedWidth(400);
    servHeader->setReadOnly(true);
    servHeader->setStyleSheet(getHeaderStyle());

    servConnect = new QLineEdit();

    viewLabel = new QLabel();
    viewLabel->setText(getViewLabel(startingViewNum()));


    // Lay out the widgets to appear in the main window.
    // For Qt widget and layout concepts see:
    // http://doc.qt.nokia.com/4.7-snapshot/widgets-and-layouts.html
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(viewLabel);
    layout->addWidget(servHeader);
    layout->addWidget(servConnect);
    layout->addWidget(textview);
    layout->addWidget(msgHeader);
    layout->addWidget(textline);
    setLayout(layout);


}

void ChatDialog::setPosition(int x, int y)
{
    this->move(x,y);
}

void ChatDialog::connectSignals()
{

    // Register a callback on the textline's returnPressed signal
    // so that we can send the message entered by the user.
    connect(textline, SIGNAL(returnPressed()),this, SLOT(gotReturnPressed()));
    connect (servConnect, SIGNAL(returnPressed()), this, SLOT(serverSelected()));
    connect (sock, SIGNAL(displayReturnedMsg(QByteArray,quint32)), this, SLOT(receivedMessageBack(QByteArray,quint32)));
    connect (sock, SIGNAL(displayViewChange(quint32)), this, SLOT(receivedViewChange(quint32)));
}

void ChatDialog::receivedViewChange(quint32 viewNum)
{
    textview->append("View Change to " + QString::number(viewNum));
    viewLabel->setText(getViewLabel(viewNum));
}

void ChatDialog::receivedMessageBack(QByteArray msg, quint32 viewNum)
{
    qDebug() << msg;
    textview->append("Received: " + QString(msg) + " (View " + QString::number(viewNum) + ")");
}

void ChatDialog::serverSelected()
{
    QString server = servConnect->text();
    quint16 port = server.toInt();

    if (port > 0)
    {
        emit this->servSelected(QHostAddress(QHostAddress::LocalHost), port);
        //qDebug() << "Server selected: " + QString::number(port);
    }
    else
    {
        qDebug() << "Bad port number";
    }

    servConnect->clear();
}

QChar randomAsciiChar()
{
    //The range of ascii characters this returns is between SPACE character and ~
    return QChar::fromAscii(32 + rand() % (127 - 32));
}

QString ChatDialog::getFillerMsg()
{
    srand (time(NULL));

    QString msg = "";

    for (quint32 i = 0; i < slotLength; i++)
    {
        msg = msg + QString(randomAsciiChar());
    }
    return msg;
}


//TODO: Still needs to hook into NetSocket
void ChatDialog::gotReturnPressed()
{
    // Initially, just echo the string locally.
    // Insert some networking code here...
    //qDebug() << "FIX: send message to other peers: " << textline->text();

    if (textline->text().length() == slotLength)
    {
        textview->append("Sent: " + textline->text());
        emit this->msgInputted(textline->text());

        // Clear the textline to get ready for the next input message.
        textline->clear();


    }
    else
    {
        QString msg = getFillerMsg();
        emit this->msgInputted(msg);

        textview->append("Filler message sent: " + msg);
    }
}
