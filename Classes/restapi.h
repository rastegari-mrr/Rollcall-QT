#ifndef RESTAPI_H
#define RESTAPI_H

#include <QObject>
#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QNetworkReply>

class RestAPI : public QObject
{
    Q_OBJECT
public:
    explicit RestAPI(QObject *parent = nullptr);

    Q_INVOKABLE void startGet(const QString &function, const QString &queryString);

signals:
    void replied(const QString &json);
    void timeout(const QString &messange = "");
    void error(const QString &messange);

public slots:

private slots:

private:
    QNetworkAccessManager *restclient;
    QNetworkReply *m_reply;
    QTimer *m_timer;
};

#endif // RESTAPI_H
