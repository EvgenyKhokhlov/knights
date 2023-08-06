#include "knife.h"

Knife::Knife()
{
    mutex = new QMutex;
    putBack();
}

void Knife::putBack()
{
    mutex->lock();
    inUse = false;
    mutex->unlock();
}

bool Knife::tryToTake()
{
    if(mutex->tryLock()){
        if(!inUse){
            inUse = true;
            mutex->unlock();
            return true;
        }
        mutex->unlock();
    }
    return false;
}
