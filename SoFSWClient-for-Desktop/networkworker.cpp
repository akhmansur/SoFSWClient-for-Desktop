#include "networkworker.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QThread>
#include <QUrlQuery>

void NetworkWorker::setLabelData(QMap<QString, QString> m_data){
    data=m_data;
}

void NetworkWorker::replyFinished(QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "ERROR: " << reply->errorString();
    }
    else
    {
        QByteArray responseData = reply->readAll();
        //QString qstr(responseData);
        //qstr=qstr.fromUtf8(responseData);
        //qDebug() << "SUCCESS: " << qstr;
        reply->deleteLater();
        emit sendResult(responseData);
        emit finished();
    }
}
 void NetworkWorker::postRequest()
 {
     QNetworkAccessManager *manager = new QNetworkAccessManager(this);
     QUrl url("http://sofsw.jabbergames.ru/g.php");
     QNetworkRequest request(url);
     QUrlQuery params;
     QByteArray postData;
     QStringList keys = data.keys();
     for(int i = 0; i < keys.size(); i++)
     {
         params.addQueryItem(keys[i],data[keys[i]]);
     }
     postData.append(params.toString().toUtf8());
     connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));
     QNetworkReply *reply = manager->post(request,postData);
     if(reply->error() != QNetworkReply::NoError)
         emit error("NET SVYAZI, ALOO");
 }
