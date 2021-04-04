#include "filedownloader.h"
#include <QPixmap>

FileDownloader::FileDownloader(QObject *parent) :
    QObject(parent)
{    
    m_timer = new QTimer(this);
    m_timer->setInterval(20000);
    m_timer->setSingleShot(true);

    connect(m_timer, &QTimer::timeout, this , [=]() {
        qDebug() << "timeout";
        m_reply->deleteLater();
        emit timeout();
    });
}

void FileDownloader::startDownload(QUrl imageUrl)
{
    try {
        QNetworkRequest request(imageUrl);
        m_reply = m_WebCtrl.get(request);
        m_timer->start();
        connect(m_reply,&QNetworkReply::finished, this, &FileDownloader::fileDownloaded);

        connect(m_reply,&QNetworkReply::downloadProgress, this, [=](qint64 bytesReceived, qint64 bytesTotal){
            m_timer->stop();
            m_timer->start();
            emit progressChanged(bytesReceived, bytesTotal);
        });

        connect(m_reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
                [=](QNetworkReply::NetworkError code)
        {
            m_timer->stop();
            Q_UNUSED(code);
            m_reply->deleteLater();
            emit error(m_reply->errorString());
        });

    } catch (std::exception ex) {
        m_timer->stop();
        if(m_reply != nullptr)
            m_reply->deleteLater();
        emit error(ex.what());
    }
}

FileDownloader::~FileDownloader() { }

void FileDownloader::fileDownloaded() {
    m_timer->stop();
    m_DownloadedData = m_reply->readAll();
    //emit a signal
    m_reply->deleteLater();
    emit downloaded(m_DownloadedData);
}

QByteArray FileDownloader::downloadedData() const {
    return m_DownloadedData;
}
