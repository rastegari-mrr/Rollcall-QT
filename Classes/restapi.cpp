#include "restapi.h"
#include <QNetworkRequest>
#include <QSslConfiguration>

RestAPI::RestAPI(QObject *parent) : QObject(parent)
{    
    restclient = new QNetworkAccessManager(this);
    m_timer = new QTimer(this);
    m_timer->setInterval(10000);
    m_timer->setSingleShot(true);

    connect(m_timer, &QTimer::timeout, this , [=]() {
        qDebug() << "timeout";
        m_reply->deleteLater();
        emit timeout();
    });
}

void RestAPI::startGet(const QString &function, const QString &queryString)
{
    try {
        QUrl myurl;
        QNetworkRequest request;
        myurl.setScheme("https");
        myurl.setHost("rcapi.omega-co.ir");
        if(function == "")
            myurl.setPath("/home");
        else
            myurl.setPath("/home/" + function);
        myurl.setQuery(queryString);
        request.setUrl(myurl);
        QSslConfiguration conf = request.sslConfiguration();
        conf.setPeerVerifyMode(QSslSocket::VerifyNone);
        request.setSslConfiguration(conf);
        m_reply = restclient->get(request);
        m_timer->start();
        qDebug() << "get request sent";
        connect(m_reply, &QNetworkReply::finished, this, [=] {
            try {
                m_timer->stop();
                auto ret = m_reply->readAll();
                m_reply->deleteLater();
                emit replied(ret);
            } catch (std::exception ex) {
                emit error(ex.what());
             }
        });

        connect(m_reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
                [=](QNetworkReply::NetworkError code)
        {            
            m_timer->stop();
            Q_UNUSED(code);
            m_reply->deleteLater();
            emit error(m_reply->errorString());
        });
    }
    catch(std::exception e) {
        m_timer->stop();
        if(m_reply != nullptr)
            m_reply->deleteLater();
        emit error(e.what());
    }
}


