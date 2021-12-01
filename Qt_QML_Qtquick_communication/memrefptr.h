#ifndef MEMREFPTR_H
#define MEMREFPTR_H

#include <QString>
#include <QObject>
#include <QThreadStorage>
#include <QMap>

class memRefPtr
{

public:

    memRefPtr();

    void ltsRegisterObject(const QString &key, QObject *object);
    void ltsRemoveObject(const QString &key);
    QObject* ltsGetObject(const QString &key);
    template <typename T> T* ltsGet(const QString &key) {
         return qobject_cast<T*>(ltsGetObject(key));
    }

};

#endif // MEMREFPTR_H
