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
    int slotLength; //How long this person's slot is in the overall message string
    void connectSignals();

    void setPosition(int x, int y);

public slots:
    void gotReturnPressed();
    void serverSelected();


    void receivedMessageBack(QByteArray msg, quint32 viewNum);
    void receivedViewChange(quint32 viewNum);

signals:
    void msgInputted(QString msg);
    void servSelected(QHostAddress address, quint16 port);


private:
    QTextEdit *textview;
    QLineEdit *textline;
    QLineEdit *servConnect;
    QLabel *viewLabel;



    QString getFillerMsg();
};
#endif // CHATDIALOG_HH
