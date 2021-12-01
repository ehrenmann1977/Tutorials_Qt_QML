#include "memrefptr.h"
/* this class stores memory pointers
 * in order to access qml functions, we need to know the pointer to pageone
 * that was created in main.cpp, and at the same time, no need to pass the pointer
 * every time to any internal class or to the backend.
 * Thus an intermemory reference is created to mange this operation
 */

static QThreadStorage<QMap<QString, QObject*> > s_qtObjects;

memRefPtr::memRefPtr()
{

}

void memRefPtr::ltsRegisterObject(const QString &key, QObject *object)
{
        s_qtObjects.localData().insert(key, object);
}

void memRefPtr::ltsRemoveObject(const QString &key)
{
    if (!s_qtObjects.hasLocalData())
        return;

    s_qtObjects.localData().remove(key);
}

QObject* memRefPtr::ltsGetObject(const QString &key)
{
    auto it = s_qtObjects.localData().find(key);
    return it != s_qtObjects.localData().end() ? it.value() : nullptr;
}

