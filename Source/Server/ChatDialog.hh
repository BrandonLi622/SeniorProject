#ifndef CHATDIALOG_HH
#define CHATDIALOG_HH

#include "CommonIncludes.hh"
#include "NetSocket.hh"

class NetSocket;


class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    ChatDialog(quint32 slotLength);
    NetSocket *sock;
    quint32 slotLength; //How long this person's slot is in the overall message string
    void connectSignals();

    void setPosition(int x, int y);

public slots:
    void receivedClientMessage(QByteArray msg, quint32 id, quint32 viewNum);
    void confirmedViewChange(quint32 viewNum, bool isMember);
    void voteOnView();

signals:
    void sendViewVote(quint32 viewNum);

private:
    QTextEdit *textview;
    QLineEdit *newViewNum;
    QLabel *viewLabel;
};
#endif // CHATDIALOG_HH
