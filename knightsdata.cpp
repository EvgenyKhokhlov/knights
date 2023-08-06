#include "knightsdata.h"

KnightsData::KnightsData(QObject *parent) : QObject(parent)
{
    knightsNames << "John" << "Rob" << "Artur" << "Clark" << "Luter";

    // Инициируем исходные значения
    for(int i = 0; i < knightsNames.count(); i++){
        knightsHunger.append(0);
        mealsNumber.append(0);
        storiesToldNumber.append(0);
        knightsCondition.append(4);
        tempCondition.append(0);
        leftKnifeInHand.append(false);
        rightKnifeInHand.append(false);
    }
}

QVector<QString> KnightsData::getNames() const
{
    return knightsNames;
}

QVector<int> KnightsData::getHunger() const
{
    return knightsHunger;
}

QVector<int> KnightsData::getCondition() const
{
    return knightsCondition;
}

QVector<int> KnightsData::getMealsNumber() const
{
    return mealsNumber;
}

QVector<int> KnightsData::getStoriesToldNumber() const
{
    return storiesToldNumber;
}

QVector<bool> KnightsData::getLeftKnifeInHand() const
{
    return leftKnifeInHand;
}

QVector<bool> KnightsData::getRightKnifeInHand() const
{
    return rightKnifeInHand;
}

int KnightsData::getKnightsCount() const
{
    return knightsNames.count();
}

void KnightsData::knightStateChanged(int id, int hunger, int condition)
{
    if(condition != 4)
        emit knightState(id, condition);

    // Обновляем данные голода
    if(knightsHunger.at(id) != hunger) knightsHunger[id] = hunger;
    if(knightsCondition.at(id) != condition){
        // Обновляем количество приемов пищи
        if(condition == 0){
            tempCondition[id] = 0;
            mealsNumber[id]++;
        }
        // Обновляем количество рассказанных историй
        if(condition == 1){
            if(tempCondition.at(id) == 0)
                storiesToldNumber[id]++;
            tempCondition[id] = 1;
        }
        // Обновляем состояние ножей
        if(condition == 2) leftKnifeInHand[id] = true;
        if(condition == 3) rightKnifeInHand[id] = true;
        if(condition == 1 || condition == 4){
            leftKnifeInHand[id] = false;
            rightKnifeInHand[id] = false;
        }

        knightsCondition[id] = condition;
    }
}
