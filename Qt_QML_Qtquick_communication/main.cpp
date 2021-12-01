#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <memcontrol.h>
#include <QQuickView>
#include <QQmlContext>
#include <QObject>
#include <backend.h>
#include <memrefptr.h>
#include <QString>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    //Backend class Initialization
    backend Backend;
    qmlRegisterType<backend>("com.drOmranConsulting.backend",1,0,"BackEnd");

    //Qml read c++ values
    engine.rootContext()->setContextProperty("_aString", QString("MV"));

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
           return -1;


    //Qml -> C++ connection
    // connect rectangle change coordinate signal coming from qml to backend dataChange_from_qml slot
    QObject *rootObject = engine.rootObjects().first();
    QObject *pageOne = rootObject->findChild<QObject*>("page1box");
    QObject::connect(pageOne, SIGNAL(changecoordinate(double,double)),&Backend,SLOT(dataChange_from_qml(double,double)));



    //C++ -> Qml SIGNAL SIGNAL Connection
    QObject::connect(&Backend,SIGNAL(informQmlXChange(double)),pageOne,SIGNAL(qmlReceive(double)));


    //Use local thread object to store pageOne pointer in order to access local qml functions
    //from backend class
    memRefPtr memobj;
    memobj.ltsRegisterObject("page1box",pageOne);

    return app.exec();

}
