#ifndef KNIFE_H
#define KNIFE_H

#include <QMutex>
#include <QDebug>

class Knife
{
public:
    Knife();

    void putBack();
    bool tryToTake();

    int id;

private:
    QMutex *mutex;
    bool inUse;
};

#endif // KNIFE_H
