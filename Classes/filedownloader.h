#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>

class FileDownloader : public QObject
{
    Q_OBJECT

public:
    explicit FileDownloader(QObject *parent = nullptr);

    Q_INVOKABLE void startDownload(QUrl imageUrl);
    virtual ~FileDownloader();
    QByteArray downloadedData() const;


signals:
    void downloaded(const QByteArray &data);
    void timeout(const QString &message = "");
    void error(const QString &message);
    void progressChanged(qint64 bytesReceived, qint64 bytesTotal);

private slots:
    void fileDownloaded();

private:
    QNetworkAccessManager m_WebCtrl;
    QNetworkReply *m_reply;
    QByteArray m_DownloadedData;
    QTimer *m_timer;
};

#endif // FILEDOWNLOADER_H
