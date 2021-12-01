#include "memcontrol.h"
#include <QtDebug>



class memcontrolData : public QSharedData
{
public:

};

memcontrol::memcontrol(QObject *parent) : QObject(parent), data(new memcontrolData)
{
    qDebug() << "starting class";
}

memcontrol::memcontrol(const memcontrol &rhs) : data(rhs.data)
{

}

memcontrol &memcontrol::operator=(const memcontrol &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

memcontrol::~memcontrol()
{

}

float memcontrol::getRectX()
{
    coordinate_x=0.1;
    qDebug() << "c++ got call from qml";
    return coordinate_x;
}

// this function is called from inside qml to inform x value inside the cpp class

void memcontrol::setRectX(float currX)
{
    qDebug() << "current x = " << m_X;

    if (currX == 0)
            return;

        m_X = currX;
        emit posChanged();
}

void memcontrol::fun (double x, double y)
{
   qDebug() << "c++ "<< x << y;
}


void memcontrol::clickme(void)
{
    qDebug() << "c++ catched the click from qml";
}
