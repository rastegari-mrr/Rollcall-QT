#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <Classes/filedownloader.h>
#include <Classes/restapi.h>
#include <Classes/useroperate.h>
#include <Classes/imageprovider.h>
#include <QUrlQuery>
#include <QDebug>
#include <QScopedPointer>

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    //    FileDownloader f;
    //    f.startDownload(QUrl("http://omega-co.ir/Pages/Products/List/JetPrinter/OnixX100/01.png"));
    //    RestAPI api;
    //    api.startGet("", "");

    qmlRegisterType<RestAPI>("Omega", 1, 0, "RestAPI");
    qmlRegisterType<UserOperate>("Omega", 1, 0, "UserOperate");

    QQmlApplicationEngine engine;

    QScopedPointer<ImageProvider> imageProvider(new ImageProvider());
    engine.rootContext()->setContextProperty(QStringLiteral("imageProvider"), imageProvider.data());
    engine.addImageProvider("myprovider", imageProvider.data());

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
