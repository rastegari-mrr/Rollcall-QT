#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QApplication>

#include <Classes/filedownloader.h>
#include <Classes/restapi.h>
#include <Classes/chequenotify.h>
#include <Classes/useroperate.h>
#include <Classes/imageprovider.h>
#include <Classes/readcart.h>

#include <QUrlQuery>
#include <QDebug>
#include <QScopedPointer>

int main(int argc, char *argv[])
{
    if(argc > 1) {
        QApplication noGUI(argc, argv);
        ReadCart *card = new ReadCart(argv[2]);
        noGUI.installEventFilter(card);
        card->resize(1, 1);
        card->move(-10000, -10000);
        card->showMinimized();
        card->hide();
        QTextStream(stdout) << "\n\r" << "\n\r";
        if(strcmp(argv[1], "set") == 0) {
            QTextStream(stdout) << "Waiting for Card..." << endl;
        } else if(strcmp(argv[1], "clear") == 0) {
            card->clear();
        }
        else {
            QTextStream(stdout) << "Invalid argument" << endl;
            return -1;
        }                
        return noGUI.exec();
    } else {

        qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

        QGuiApplication app(argc, argv);

        //    FileDownloader f;
        //    f.startDownload(QUrl("http://omega-co.ir/Pages/Products/List/JetPrinter/OnixX100/01.png"));
        //    RestAPI api;
        //    api.startGet("", "");

        qmlRegisterType<RestAPI>("Omega", 1, 0, "RestAPI");
        qmlRegisterType<UserOperate>("Omega", 1, 0, "UserOperate");
        qmlRegisterType<ChequeNotify>("Omega", 1, 0, "ChequeNotify");

        QQmlApplicationEngine engine;

        QScopedPointer<ImageProvider> imageProvider(new ImageProvider());
        engine.rootContext()->setContextProperty(QStringLiteral("imageProvider"), imageProvider.data());
        engine.addImageProvider("myprovider", imageProvider.data());

        engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
        if (engine.rootObjects().isEmpty())
            return -1;

        return app.exec();
    }
}
