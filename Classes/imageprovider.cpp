#include "imageprovider.h"
#include <QBuffer>
#include <QUrl>
#include <QUrlQuery>
#include <QDebug>

ImageProvider::ImageProvider() : QQuickImageProvider (QQuickImageProvider::Image)
{
    this->blockSignals(false);
}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QImage result;
    if(id.startsWith("extern"))
    {
        QUrl url(id);
        QUrlQuery query(url.query());
        auto path = query.queryItemValue("path");
        result = QImage(path);

        if(result.isNull())
        {
            if(requestedSize.width() > 0 && requestedSize.height() > 0)
                return QImage(requestedSize.width(), requestedSize.height(), QImage::Format_ARGB32);
            else
                return QImage(100, 100, QImage::Format_ARGB32);
        }

        if(size)
            *size = result.size();
    }
    else if(id.startsWith("fromJson"))
    {
        QUrl url(id);
        QUrlQuery query(url.query());
        auto json = query.queryItemValue("json");
        auto const encoded = json.toLatin1();
        QImage p;
        p.loadFromData(QByteArray::fromBase64(encoded), "PNG");

        result = p;

        if(result.isNull())
        {
            if(requestedSize.width() > 0 && requestedSize.height() > 0)
                return QImage(requestedSize.width(), requestedSize.height(), QImage::Format_ARGB32);
            else
                return QImage(100, 100, QImage::Format_ARGB32);
        }

        if(size)
            *size = result.size();

        if(!result.isNull() && requestedSize.width() > 0 && requestedSize.height() > 0)
            result = result.scaled(requestedSize.width(), requestedSize.height(), Qt::IgnoreAspectRatio);
    }


    return result;

}

void ImageProvider::updateImage(const QImage &image)
{
    if(this->image != image)
    {
        this->image = image;
        //this->image.save("aaa.png");
        emit imageChanged();
    }
}


QJsonValue ImageProvider::jsonValFromImage(const QImage &p)
{
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    p.save(&buffer, "PNG");
    //p.save("000.png");
    auto const encoded = buffer.data().toBase64();
    return {QLatin1String(encoded)};
    //updateImage(p);
}





