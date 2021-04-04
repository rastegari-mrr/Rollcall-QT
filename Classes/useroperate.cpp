#include "useroperate.h"
#include <QFile>
#include <QJsonObject>
#include <QJsonValue>
#include <QPixmap>
#include "runlinuxcommands.h"

UserOperate::UserOperate(QObject *parent) : QObject(parent)
{
    userImages = new QList<UserImageFiles>();
    m_userGetWatcher = new QFutureWatcher<void>(this);
    m_fileDownloader = new FileDownloader();

    connect(m_fileDownloader, &FileDownloader::downloaded, this, [=](const QByteArray &data) {
        QPixmap bitmap;
        bitmap.loadFromData(data);
        bitmap.save(userImages->at(m_currentDownloadIndex).saveFileName);
        //qDebug() << userImages->at(m_currentDownloadIndex).saveFileName << " saved";
        m_currentDownloadIndex++;
        checkForNewDownload();
    });

    connect(m_fileDownloader, &FileDownloader::progressChanged, this, [=](qint64 bytesReceived, qint64 bytesTotal) {
        auto percent = static_cast<int>(static_cast<double>(bytesReceived) / static_cast<double>(bytesTotal) * 100);
        //qDebug() << "percent : " << m_currentDownloadIndex << " , " << percent;
        emit progressChanged(percent, m_currentDownloadIndex + 1, userImages->length());
    });

    connect(m_fileDownloader, &FileDownloader::timeout, this, [=] {
        emit timeout();
    });

    connect(m_userGetWatcher, &QFutureWatcher<void>::finished, this, [=] {
        //qDebug() << "finished";
        m_currentDownloadIndex = 0;
        checkForNewDownload();
    });
}

void UserOperate::getUsersPhoto(const QVariant &users, bool forceRewitten)
{
    QString path = QString("/home/pi/");
    //auto fu = QtConcurrent::run([=] {
    try {
        userImages->clear();
        auto usersList = users.toList();
        for (int i = 0; i < usersList.length(); i++) {
            auto user = usersList.at(i).toJsonObject();
            auto image = user["image"].toString();
            auto imageFileName = path + QString::number(user["id"].toInt()) + ".jpg";
            if(forceRewitten) {
                if(QFile::exists(imageFileName))
                    deleteFile(imageFileName);
                if(image != "") {
                    UserImageFiles file;
                    file.downloadAddress = image;
                    file.saveFileName = imageFileName;
                    userImages->append(file);
                }
            } else {
                if(image != "" && !QFile::exists(imageFileName)) {
                    UserImageFiles file;
                    file.downloadAddress = image;
                    file.saveFileName = imageFileName;
                    userImages->append(file);
                }
            }
        }
    } catch(std::exception ex) {
        qDebug() << ex.what();
    }
    //});
    //m_userGetWatcher->setFuture(fu);

    m_currentDownloadIndex = 0;
    checkForNewDownload();
}

bool UserOperate::fileExists(const QString &fileName)
{
    return QFile::exists(fileName);
}

bool UserOperate::saveFile(const QString &fileName, const QString &data)
{
    try {
        QFile file(fileName);
        if(!file.open(QIODevice::Text | QIODevice::WriteOnly))
            return false;
        QTextStream stream(&file);
        stream << data;
        file.close();
        QFile::setPermissions(fileName, QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner |
                              QFileDevice::ReadUser | QFileDevice::WriteUser | QFileDevice::ExeUser |
                              QFileDevice::ReadGroup | QFileDevice::WriteGroup | QFileDevice::ExeGroup |
                              QFileDevice::ReadOther | QFileDevice::WriteOther | QFileDevice::ExeOther);
        sync();
        return true;
    } catch (...) {
        return false;
    }
}



QString UserOperate::readFile(const QString &fileName) const
{
    try {
        if(!QFile::exists(fileName))
            return "";
        QFile file(fileName);
        QString command = "sudo";
        QStringList args;
        args << "cat" << fileName;
        auto ret = RunLinuxCommands::run(command, args);
        if(ret.error != "")
            return "";
        else {
            return ret.output;
        }
    } catch (...) {
        return "";
    }
}

QString UserOperate::getIP() const
{
    QString command;
    QStringList args;
    command = "hostname";
    args << "-I";
    auto ret = RunLinuxCommands::run(command, args);
    if(ret.output.contains('.'))
        return ret.output;
    else
        return "Not Connected";
}

QString UserOperate::setDateTime(const QDateTime &dateTime) const
{
    QString command = "sudo";
    QStringList dateArgs;
    QStringList timeArgs;
    QString date, time;
    date.sprintf("%04d%02d%02d", dateTime.date().year(), dateTime.date().month(), dateTime.date().day());
    dateArgs << "date" << "+%Y%m%d" << "-s" << date;
    auto ret = RunLinuxCommands::run(command, dateArgs);
    if(ret.error != "")
        return ret.error;
    //qDebug() << dateTime << date << ret.output << ret.error;
    time.sprintf("%02d:%02d:%02d", dateTime.time().hour(), dateTime.time().minute(), dateTime.time().second());
    timeArgs << "date" << "+%T" << "-s" << time;
    ret = RunLinuxCommands::run(command, timeArgs);
    return ret.error;
    //qDebug() << ret.output << ret.error;
}

bool UserOperate::deleteFile(const QString &fileName)
{
    QString command = "sudo";
    QStringList args;
    args << "rm" << fileName;
    auto ret = RunLinuxCommands::run(command, args);
    sync();
    return ret.error == "";
}

void UserOperate::sync()
{
    QString command;
    QStringList args;
    command = "sudo";
    args << "sync";
    RunLinuxCommands::run(command, args);
}

void UserOperate::checkForNewDownload()
{
    if(m_currentDownloadIndex < userImages->length())
        m_fileDownloader->startDownload(userImages->at(m_currentDownloadIndex).downloadAddress);
    else {
        emit downloadsFinished();
    }
}
