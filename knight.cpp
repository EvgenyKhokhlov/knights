#include "knight.h"

Knight::Knight(int knightID, QString knightName)
{
    id = knightID;
    name = knightName;

    hungerLevelPercent = 100; // Стартовый уровень голода
    isHungry = false;
    isSimulationAllowed = false;
    isEatingAllowed = true; // Изначально всем рыцарям разрешен прием пищи
}

int Knight::getId() const
{
    return id;
}

QString Knight::getName() const
{
    return name;
}

void Knight::setLeftKnife(Knife *knife)
{
    leftKnife = knife;
}

void Knight::setRightKnife(Knife *knife)
{
    rightKnife = knife;
}

Knight::Condition Knight::getCondition() const
{
    return condition;
}

int Knight::getHungerLevel() const
{
    return hungerLevelPercent;
}

void Knight::setIsSimulationAllowed(bool value)
{
    isSimulationAllowed = value;
}

void Knight::setIsEatingAllowed(int knightID, bool value)
{
    if(knightID != id) return;

    isEatingAllowed = value;
}

void Knight::run()
{    
    while (true) {
        if(!isSimulationAllowed){
            if(condition != Idle){
                condition = Idle;
                emit stateChanged(id, hungerLevelPercent, condition);
            }
            continue;
        }

        if(hungerLevelPercent > minHungerLevel)
            isHungry = true;
        else
            isHungry = false;

        if(isEatingAllowed && isHungry){
            if(leftKnife->tryToTake()){ // Пытаемся взять левый нож
                condition = LeftKnifeTaking;
                emit stateChanged(id, hungerLevelPercent, condition);
                usleep(1000000 * knifeTakingTimeSecs);
                if(rightKnife->tryToTake()){ // Левый нож взяли, пытаемся взять правый
                    condition = RightKnifeTaking;
                    emit stateChanged(id, hungerLevelPercent, condition);
                    usleep(1000000 * knifeTakingTimeSecs);
                    // Едим пока не наедимся, запущена симуляция и алгоритм разрешает
                    while(hungerLevelPercent > 0 && isSimulationAllowed && isEatingAllowed){
                        condition = Eating;
                        updateHungerLevel();
                        emit stateChanged(id, hungerLevelPercent, condition);
                        usleep(1000000 * updateTimeSecs);
                    }
                    // Прекратили прием пищи, ложим ножи на место
                    leftKnife->putBack();
                    rightKnife->putBack();
                    continue;
                }else{
                    leftKnife->putBack(); // Не получилось взять правый нож, ложим левый на место
                }
            }
        }
        condition = StoryTelling;
        updateHungerLevel();
        emit stateChanged(id, hungerLevelPercent, condition);
        usleep(1000000 * updateTimeSecs);
    }
}

void Knight::updateHungerLevel()
{
    if(condition == Eating){
        hungerLevelPercent -= hungerDecreaseSpeed;
        if(hungerLevelPercent < 0) hungerLevelPercent = 0;
        return;
    }
    if(condition == StoryTelling){
        hungerLevelPercent += hungerIncreaseSpeed;
        if(hungerLevelPercent > 100) hungerLevelPercent = 100;
    }
}
