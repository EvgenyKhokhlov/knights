#include "knight.h"

Knight::Knight(int knightID, QString knightName)
{
    id = knightID;
    name = knightName;

    hungerLevelPercent = 100;
    condition = Idle;
    isHungry = false;
    isSimulationAllowed = false;
    isEatingAllowed = true; //TODO вернуть false после тестов
}

int Knight::getId() const
{
    return id;
}

QString Knight::getName() const
{
    return name;
}

void Knight::setLeftKnife(Knife *value)
{
    leftKnife = value;
}

void Knight::setRightKnife(Knife *value)
{
    rightKnife = value;
}

Knight::Condition Knight::getCondition() const
{
    return condition;
}

int Knight::getHungerLevel() const
{
    return hungerLevelPercent;
}

void Knight::run()
{    
    while (true) {
        if(!isSimulationAllowed){
            condition = Idle;
            continue;
        }

        if(hungerLevelPercent > 20)
            isHungry = true;
        else
            isHungry = false;

        if(isEatingAllowed && isHungry){
            if(leftKnife->tryToTake()){
                emit stateChanged(QString(name + " take left knife"));
                if(rightKnife->tryToTake()){
                    emit stateChanged(QString(name + " take right knife"));
                    while(hungerLevelPercent > 0 && isSimulationAllowed){
                        condition = Eating;
                        emit stateChanged(QString(name + " eating"));
                        hungerLevelPercent -= 10;

                        usleep(1000000 * updateTimeSecs);
                    }
                    leftKnife->putBack();
                    rightKnife->putBack();
                    continue;
                }else{
                    leftKnife->putBack();
                }
            }
        }
        emit stateChanged(QString(name + " telling story"));
        condition = StoryTelling;
        hungerLevelPercent += 5;
        if(hungerLevelPercent > 100) hungerLevelPercent = 100;
        usleep(1000000 * updateTimeSecs);
    }
}
