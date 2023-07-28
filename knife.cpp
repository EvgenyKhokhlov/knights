#include "knife.h"

Knife::Knife()
{
    putBackMutex = new QMutex;
    takeMutex = new QMutex;
    putBack();
}

void Knife::putBack()
{
    putBackMutex->lock();
    inUse = false;
    putBackMutex->unlock();
}

bool Knife::tryToTake()
{
    if(takeMutex->tryLock()){
        if(!inUse){
            inUse = true;
            takeMutex->unlock();
            return true;
        }
        takeMutex->unlock();
    }
    return false;
}
