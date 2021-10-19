#include "readcart.h"
#include <QJsonObject>
#include <QJsonDocument>

ReadCart::ReadCart(const QString &id, QWidget *parent) : QWidget(parent)
{
    m_id = id;
    api = new RestAPI(this);
    connect(api, &RestAPI::error, this, [=](const QString &str) {
        QTextStream(stdout) << "Error in running Rest API : " << str << endl;
        exit(EXIT_FAILURE);
    });

    connect(api, &RestAPI::timeout, this, [=](const QString &str) {
        QTextStream(stdout) << "Rest API timeout. Error : " << str << endl;
        exit(EXIT_FAILURE);
    });

    connect(api, &RestAPI::replied, this, [=](const QString &str) {
        QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
        auto json = doc.object();
        if(json.value("success").toString() == "true") {
            QTextStream(stdout) << "Done successfully" << endl;
            exit(EXIT_SUCCESS);
        } else {
            QTextStream(stdout) << "Error in Rest API : " << json.value("message").toString() << endl;
            exit(EXIT_FAILURE);
        }
    });
}

void ReadCart::clear()
{
    QString str = QString("id=%0&cardNo=").arg(m_id);
    api->startGet("SaveCardNo", str);
}
