#include <backend.h>
#include <QtDebug>
#include <QTimer>


backend::backend(QObject *parent) : QObject(parent)
{
    /* setting a timer each 1 sec */
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout,this, QOverload<>::of(&backend::timerOut));
    timer->start(5000);
}


/* dataChange_from_qml:
 * this function is a slot bound in main.cpp to a signal changecoordinate that is emitted
 * from inside main.QML when the position is changed.
 *
 * Dataflow: QML -> C++
 */
void backend::dataChange_from_qml (double x, double y)
{
   qDebug() << "backend.cpp: dataChange_from_qml: Received signal from QML with datachange from:"<< rec_x << " to: x= "<< x << " y=" <<  y;
   rec_x=x;     //update local values

}


/* getRecX :
 *  this function is exposed to QML when qml wants to read rec_x
 *  it lands to this function
 */
double backend::getRecX()
{
    qDebug() << "backend.cpp: getRecX : internal value of rec_x =" << rec_x;
    return rec_x;
}



/* timer management
 * a timer has been set to run at the begining of the class
 * when the timeout is emitted it comes here
 */
void backend::timerOut()
{
    qDebug() << "backend.cpp: timerOut. current x=" << rec_x << " change x to 80";
    double cusx=80;
    setRecX(cusx); //setting new value internally


    /* this is an example how to call qml function and read
     * its output. Notice that the output will be invalid
     * if the object in qml side has not been initalized
     */
    memRefPtr memobjekt;

    QObject* p1=memobjekt.ltsGetObject("page1box");
    QVariant returnedValue;
    QVariant msg = "Hello from C++";
    QMetaObject::invokeMethod(p1, "myQmlFunction", Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, msg));
    qDebug() << "QML function returned:" << returnedValue;
}



/* setRecX:
 *  this function is called in cpp from the timer to change the rec_x value
 */
void backend::setRecX(double X)
{
    //rec_x needs to be updated before the check
    qDebug() << "backend.cpp: setRecX : setting a new value to rec_x = " << X << " from "<<rec_x;
    if (X == rec_x)
    {
        qDebug() << "return";
        return;
    }
    rec_x =X;
    qDebug() << "change emitted";
    emit informQmlXChange(X);
}

