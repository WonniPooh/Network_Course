#ifndef REQUESTSENDER_H
#define REQUESTSENDER_H

#include <QtNetwork>
#include <QObject>
#include <QTime>
#include "replyhandler.h"

class RequestSender: public QObject
{
    Q_OBJECT

private:
    QTime time;
    QTimer *timer;
    static const int numTimeframes = 5;
    ReplyHandler reply_handler;

    QNetworkAccessManager* networkManager;
    QNetworkRequest requests[numTimeframes];

    QVector<int> current_quotes;
    int timeframes[numTimeframes] = {60, 300, 900, 1800, 3600};

    void initRequests();
    void formRequest(int frame_num);
    void testServerConnection();

public slots:
    void sendRequests();

signals:
    void doItAgain();

public:
    RequestSender(QObject* pobj = 0);
    ~RequestSender();
    void addQuote(const int quote);
    void removeQuote(const int quote);
};
#endif // REQUESTSENDER_H
