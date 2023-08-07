#include "hungercontrol.h"

HungerControl::HungerControl(QVector<int> *hungerLevel, QObject *parent) : QObject(parent)
{
    hunger = hungerLevel;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(controlHunger()));
}

void HungerControl::startSimulation()
{
    timer->start(1000 * operateTimeSecs);
}

void HungerControl::stopSimulation()
{
    timer->stop();
}

void HungerControl::controlHunger()
{
    for(int i = 0; i < hunger->count(); i++){
        // Если соседние рыцари голоднее текущего на определененное значение - запрещаем ему есть
        if(hunger->at(prevID(i)) - hunger->at(i) > hungerLevelDifference ||
           hunger->at(nextID(i)) - hunger->at(i) > hungerLevelDifference){
            emit isEatingAllowed(i, false);
            continue;
        }
        emit isEatingAllowed(i, true);
    }
}

int HungerControl::nextID(int id)
{
    if(id >= hunger->count() - 1)
        return 0;
    else
        return id + 1;
}

int HungerControl::prevID(int id)
{
    if(id - 1 < 0)
        return hunger->count() - 1;
    else
        return id - 1;
}
