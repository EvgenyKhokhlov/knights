#include "hungercontrol.h"

#include <QTimer>

HungerControl::HungerControl(QVector<int> *hungerLevel, QVector<int> *knightsCondition, QObject )
{
    operateTimeSecs = 2;
    hunger = hungerLevel;
    condition = knightsCondition;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(controlHunger()));
    timer->start(1000 * operateTimeSecs);
}

void HungerControl::controlHunger()
{
    for(int i = 0; i < *hunger.count(); i++){

    }
}
