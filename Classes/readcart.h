#ifndef READCART_H
#define READCART_H

#include <QWidget>
#include <QKeyEvent>
#include "Classes/restapi.h"

class ReadCart : public QWidget
{
    Q_OBJECT
public:
    explicit ReadCart(const QString &id, QWidget *parent = nullptr);
    void clear();
signals:


public slots:

private:
    QString Card;
    RestAPI *api;
    QString m_id;

protected:
    void keyPressEvent(QKeyEvent *event) override {
        if(event->key() != Qt::Key_Return) {
            Card += event->text();
        } else {
            if(Card.length() >= 10) {
                if(api != nullptr) {
                    QString str = QString("id=%1&cardNo=%2").arg(m_id).arg(Card);
                    api->startGet("SaveCardNo", str);
                }
            }
            else {
                QTextStream(stdout) << "Invalid Card Number" << endl;
                exit(EXIT_FAILURE);
            }
        }
    }
};

#endif // READCART_H
