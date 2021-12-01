#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <memrefptr.h>

class backend : public QObject
{
    Q_OBJECT
                                    /* 1- EXPOSING CPP VALUES TO QML */
    Q_PROPERTY(double rec_x READ getRecX WRITE setRecX NOTIFY informQmlXChange)
                                    /* 1                            */

public:
    explicit backend(QObject *parent = nullptr);

                                    /* 2- EXPOSING CPP VALUES TO QML */

    Q_INVOKABLE double getRecX();   /* Qml reads current rec_x value */
    void setRecX(double X);         /* Cpp sets new rec_x value      */
                                    /* 2                             */

    void timerOut();                /* timeout event function calls  */

    double rec_x;                   /* exposed property to QML       */

signals:

    void informQmlXChange(double);  /* 3- this is a signal-signal connection to  */
                                    /* inform qml side that x-value has been     */
                                    /* changed, and thus the corresponding signal*/
                                    /* qmlReceive will call the                  */
                                    /* onQmlReceive function in qml              */

public slots:
                                    /* binding QML signal            */
    void dataChange_from_qml(double x, double y);



};

#endif // BACKEND_H
