#ifndef USEROPERATE_H
#define USEROPERATE_H

#include <QObject>
#include <QVariant>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>
#include "filedownloader.h"

struct UserImageFiles {
    QString downloadAddress;
    QString saveFileName;
};

class UserOperate : public QObject
{
    Q_OBJECT
public:
    explicit UserOperate(QObject *parent = nullptr);

    Q_INVOKABLE void getUsersPhoto(const QVariant &users, bool forceRewitten);
    Q_INVOKABLE bool fileExists(const QString &fileName);
    Q_INVOKABLE bool saveFile(const QString &fileName, const QString &data);
    Q_INVOKABLE QString readFile(const QString &fileName) const;
    Q_INVOKABLE QString getIP() const;
    Q_INVOKABLE QString setDateTime(const QDateTime &dateTime) const;

signals:
    void progressChanged(int percent, int currentFile, int totalFiles);
    void timeout();
    void downloadsFinished();

public slots:

private:
    bool deleteFile(const QString &fileName);
    void sync();
    void checkForNewDownload();

    QList<UserImageFiles> *userImages;
    QFutureWatcher<void> *m_userGetWatcher;
    int m_currentDownloadIndex;
    FileDownloader *m_fileDownloader;
};

#endif // USEROPERATE_H
