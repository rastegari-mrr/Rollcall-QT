#include "chequenotify.h"

ChequeNotify::ChequeNotify(QObject *parent) : QObject(parent)
{
    restclient = new QNetworkAccessManager(this);
    m_timer = new QTimer(this);
    m_timer->setInterval(10000);
    m_timer->setSingleShot(true);

    connect(m_timer, &QTimer::timeout, this , [=]() {
        qDebug() << "cheque timeout";
        m_reply->deleteLater();
        emit timeout();
    });
}

void ChequeNotify::startGet()
{
    try {        
        QUrl myurl;        
        myurl.setScheme("https");
        myurl.setHost("fapi.omega-co.ir");
        myurl.setPath("/api/bot/cheque");                
        QNetworkRequest request;
        request.setUrl(myurl);

        QSslConfiguration conf = request.sslConfiguration();
        conf.setPeerVerifyMode(QSslSocket::VerifyNone);
        request.setSslConfiguration(conf);        

        m_reply = restclient->get(request);
        m_timer->start();
        qDebug() << "cheque get request sent";
        connect(m_reply, &QNetworkReply::finished, this, [=] {
            try {
                m_timer->stop();
                auto ret = m_reply->readAll();
                m_reply->deleteLater();
                qDebug() << "cheque :" << ret;
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
            qDebug() << "cheque error:" << m_reply->errorString();
            emit error(m_reply->errorString());
        });
    }
    catch(std::exception e) {
        m_timer->stop();
        if(m_reply != nullptr)
            m_reply->deleteLater();
        qDebug() << "cheque error:" << e.what();
        emit error(e.what());
    }
}
