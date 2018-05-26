#include "requestsender.h"
#include <iostream>

RequestSender::RequestSender(QObject* pobj) : QObject(pobj)
{
    time.start();
    timer = new QTimer;
    networkManager = nullptr;

    QObject::connect(timer, &QTimer::timeout, this, &RequestSender::sendRequests);
    QObject::connect(&reply_handler, &ReplyHandler::doItAgain, this, &RequestSender::sendRequests);
    QObject::connect(this, &RequestSender::doItAgain, this, &RequestSender::sendRequests);
    initRequests();
}

RequestSender::~RequestSender()
{

}

void RequestSender::addQuote(const int quote)
{
    current_quotes.push_back(quote);

    for(int i = 0; i < numTimeframes; i++)
    {
        formRequest(i);
    }
}

void RequestSender::removeQuote(const int quote)
{
    current_quotes.removeOne(quote);

    for(int i = 0; i < numTimeframes; i++)
    {
        formRequest(i);
    }
}

void RequestSender::formRequest(int frame_num)
{
    QString query_url = "https://ssltsw.forexprostools.com/api.php?action=refresher&pairs=";

    int length = current_quotes.length();

    for(int i = 0; i < length; i++)
    {
        if(i+1 != length)
            query_url.append(QString::number(current_quotes[i]) + QString(","));
        else
            query_url.append(QString::number(current_quotes[i]));
    }

    query_url.append("&timeframe=");
    query_url.append(QString::number(timeframes[frame_num]));
    requests[frame_num].setUrl(QUrl(query_url));
}

void RequestSender::initRequests()
{
    for (int i = 0; i < numTimeframes; ++i)
    {
        requests[i].setRawHeader("Host", "ssltsw.forexprostools.com");
        requests[i].setRawHeader("Accept-Encoding", "deflate");
        requests[i].setRawHeader("Connection", "keep-alive");
        requests[i].setRawHeader("Referer", "https://ssltsw.forexprostools.com/");
    }
}

void RequestSender::sendRequests()
{
    if(networkManager == nullptr)
        networkManager = new QNetworkAccessManager(this);


    if(current_quotes.length())
    {
        if(time.elapsed() > 800)
        {
            for (int i = 0; i < numTimeframes; i++)
            {
                QNetworkReply* reply = networkManager -> get(requests[i]);
                QObject::connect(reply, &QNetworkReply::finished, &reply_handler, &ReplyHandler::slotHandleReply);
            }

            time.restart();
            return;
        }
    }
    timer->start(800);
}
