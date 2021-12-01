#ifndef MEMCONTROL_H
#define MEMCONTROL_H

#include <QObject>
#include <QSharedDataPointer>
#include <qqml.h>

class memcontrolData;

class memcontrol : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float coordinate_x READ getRectX WRITE setRectX NOTIFY posChanged)


public:
    explicit memcontrol(QObject *parent = nullptr);
    memcontrol(const memcontrol &);
    memcontrol &operator=(const memcontrol &);
    ~memcontrol();

    float coordinate_x; //initial x coordinate
    Q_INVOKABLE float getRectX(); //read coord X from qml side
    void setRectX(float currX); //set new coord x in Cpp side
    void clickme(void);

signals:
    void posChanged(); //when changing x coord emit the signal

public slots:
           void fun(double x, double y);

private:
    QSharedDataPointer<memcontrolData> data;

    float m_X;
};

#endif // MEMCONTROL_H
