#include "replyhandler.h"
#include <iostream>

ReplyHandler::ReplyHandler(QObject *pobj): QObject(pobj)
{
    reply_counter = 0;
    client.setUpConnection("localhost", 2323, 11003);
}

ReplyHandler::~ReplyHandler()
{

}

void ReplyHandler::slotHandleReply()
{
    QNetworkReply* reply = (QNetworkReply*)sender();
    int current_timeframe = 0;
    if ( reply->error() != QNetworkReply::NoError ) {
        qWarning() <<"ErrorNo: "<< reply->error() << "for url: " << reply->url().toString();
        qDebug() << "Request failed, " << reply->errorString();
        qDebug() << "Headers:"<<  reply->rawHeaderList()<< "content:" << reply->readAll();
        reply->deleteLater();
        return;
    }
    QString current_url = reply->url().toString();
    current_timeframe = current_url.mid(current_url.lastIndexOf('=') + 1,
                                        current_url.length() - current_url.lastIndexOf('=') - 1).toInt();

    QString reply_data = reply->readAll();
    if(reply_data.length() == 0)
    {
        reply->deleteLater();

        if(++reply_counter == numTimeframes)
        {
            reply_counter = 0;
            emit doItAgain();
        }
        return;
    }
    std::cout << "not empty";
    QJsonDocument jsonResponse = QJsonDocument::fromJson(reply_data.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();

    QJsonObject quote_obj;
    QString technicalSummary;
    int asset_id = 0;
    int maBuy = 0;
    int maSell = 0;
    int tiBuy = 0;
    int tiSell = 0;

    QJsonObject::iterator end = jsonObject.end();
    end -= 1;

    for (QJsonObject::iterator it = jsonObject.begin(); it != end; it++)
    {
        asset_id = it.key().toInt();

        quote_obj = it.value().toObject();
        maBuy = quote_obj["maBuy"].toInt();
        maSell = quote_obj["maSell"].toInt();
        tiBuy = quote_obj["tiBuy"].toInt();
        tiSell = quote_obj["tiSell"].toInt();
        technicalSummary = quote_obj["technicalSummary"].toString();

        QJsonObject json_obj;
        json_obj.insert(QString("asset_id"), QJsonValue(QString::number(asset_id)));
        json_obj.insert(QString("timeFrame"), QJsonValue(QString::number(current_timeframe)));
        json_obj.insert(QString("technicalSummary"), QJsonValue(technicalSummary));
        json_obj.insert(QString("maBuy"), QJsonValue(QString::number(maBuy)));
        json_obj.insert(QString("maSell"), QJsonValue(QString::number(maSell)));
        json_obj.insert(QString("tiBuy"), QJsonValue(QString::number(tiBuy)));
        json_obj.insert(QString("tiSell"), QJsonValue(QString::number(tiSell)));

        QJsonDocument doc(json_obj);
        QString str_json(doc.toJson(QJsonDocument::Compact));

        if(prev_data.contains(asset_id))
        {
            if(prev_data.value(asset_id) != str_json)
            {
                prev_data.insert(asset_id, str_json);
                sendJsonData(json_obj);
            }
        }
        else
        {
            prev_data.insert(asset_id, str_json);
            sendJsonData(json_obj);
        }
    }

    reply->deleteLater();

    if(++reply_counter == numTimeframes)
    {
        reply_counter = 0;
        emit doItAgain();
    }
}

void ReplyHandler::sendJsonData(QJsonObject data_to_send)
{
    QJsonObject msg_to_send;
    QJsonArray dest_id_array;
    dest_id_array.push_back(QJsonValue(11100));
    msg_to_send.insert(QString("receiver_id"), QJsonValue(dest_id_array));
    msg_to_send.insert(QString("data"), QJsonValue(data_to_send));

    QJsonDocument doc(msg_to_send);
    QString str_json(doc.toJson(QJsonDocument::Compact));
    std::cout << str_json.toStdString() << std::endl;
    client.sendMsg(str_json.toStdString());
}
