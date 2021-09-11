#ifndef CHEQUENOTIFY_H
#define CHEQUENOTIFY_H

#include <QObject>
#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QNetworkReply>

class ChequeNotify : public QObject
{
    Q_OBJECT
public:
    explicit ChequeNotify(QObject *parent = nullptr);

    Q_INVOKABLE void startGet();

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

#endif // CHEQUENOTIFY_H
