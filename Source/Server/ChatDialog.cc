#include "ChatDialog.hh"


quint32 startingViewNum()
{
    return 0;
}

QString getHeaderStyle()
{
    return "QLineEdit {  border: 1px solid black; height: 30}";
}

QString getViewLabel(quint32 viewNum)
{
    return "Current view: " + QString::number(viewNum);
}

ChatDialog::ChatDialog(quint32 slotLength)
{
    this->slotLength = slotLength;

    // Read-only text box where we display messages from everyone.
    // This widget expands both horizontally and vertically.
    textview = new QTextEdit(this);
    textview->setReadOnly(true);

    viewLabel = new QLabel();
    viewLabel->setText(getViewLabel(startingViewNum()));

    QLabel *viewChangeLabel = new QLabel();
    viewChangeLabel->setText("Vote on a new view:");
    newViewNum = new QLineEdit();

    QHBoxLayout *viewChangeSection = new QHBoxLayout();
    viewChangeSection->addWidget(viewChangeLabel);
    viewChangeSection->addWidget(newViewNum);

    // Lay out the widgets to appear in the main window.
    // For Qt widget and layout concepts see:
    // http://doc.qt.nokia.com/4.7-snapshot/widgets-and-layouts.html
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addLayout(viewChangeSection);
    layout->addWidget(viewLabel);
    layout->addWidget(textview);
    setLayout(layout);


}


void ChatDialog::setPosition(int x, int y)
{
    this->move(x,y);
}

void ChatDialog::confirmedViewChange(quint32 viewNum, bool isMember)
{
    textview->append("View changed to " + QString::number(viewNum)  + (isMember ? "(Member)" : "(Not member)"));
    viewLabel->setText(getViewLabel(viewNum));
}

void ChatDialog::connectSignals()
{
    this->connect(sock, SIGNAL(receivedMessage(QByteArray,quint32,quint32)), this, SLOT(receivedClientMessage(QByteArray,quint32,quint32)));
    this->connect(sock, SIGNAL(displayViewChange(quint32,bool)), this, SLOT(confirmedViewChange(quint32,bool)));
    this->connect(newViewNum, SIGNAL(returnPressed()), this, SLOT(voteOnView()));
}

void ChatDialog::voteOnView()
{
    bool ok = true;
    quint32 viewNum = newViewNum->text().toUInt(&ok);

    if (ok && viewNum >= 0)
    {
        qDebug() << "New view proposed: " << viewNum;
        emit this->sendViewVote(viewNum);
    }
    else
    {
        qDebug() << "Views must be positive integers";
    }
    newViewNum->clear();
}

void ChatDialog::receivedClientMessage(QByteArray msg, quint32 id, quint32 viewNum)
{
    textview->append("Received from Client" + QString::number(id) + ": " + QString(msg) + " (View " + QString::number(viewNum) + ")");
}


