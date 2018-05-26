#ifndef REPLYHANDLER_H
#define REPLYHANDLER_H

#include <QtNetwork>
#include <QObject>
#include "tcpclient.h"

const int numTimeframes = 5;

class ReplyHandler: public QObject
{
    Q_OBJECT

signals:
    void doItAgain();

public slots:
    void slotHandleReply();

public:
    explicit ReplyHandler(QObject *pobj = 0);
    ~ReplyHandler();
private:
    void sendJsonData(QJsonObject data_to_send);
    TcpClient client;
    QMap<int, QString> prev_data;
    int reply_counter;
};
#endif // REPLYHANDLER_H
