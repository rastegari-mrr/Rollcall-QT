#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QObject>
#include <QQuickImageProvider>
#include <QImage>
#include <QJsonValue>

class ImageProvider : public QObject, public QQuickImageProvider
{
    Q_OBJECT
public:
    ImageProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize& requestedSize) override;

    Q_INVOKABLE QJsonValue jsonValFromImage(const QImage &p);    

signals:
    void imageChanged();

public slots:
    void updateImage(const QImage &image);    

private:
    QImage image;
};

#endif // IMAGEPROVIDER_H
