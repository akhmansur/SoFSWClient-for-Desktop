#ifndef NETWORKWORKER_H
#define NETWORKWORKER_H
#include <QtNetwork/QNetworkAccessManager>
#include <QThread>

class NetworkWorker: public QObject
 {
    Q_OBJECT
 public:
    void setLabelData(QMap<QString, QString> m_data);
private:
    QMap<QString, QString> data;
public slots:
    void postRequest();
     void replyFinished(QNetworkReply *);
 signals:
     void sendResult(QByteArray str);
     void error(QString err);
     void finished();

 };

#endif // NETWORKWORKER_H
