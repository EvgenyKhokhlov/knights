#ifndef KNIGHT_H
#define KNIGHT_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QDebug>

#include "knife.h"

class Knight : public QThread
{
    Q_OBJECT
public:
    Knight(int knightID, QString knightName);

    int getId() const;
    QString getName() const;

    enum Condition{
        Eating           = 0,
        StoryTelling     = 1,
        LeftKnifeTaking  = 2,
        RightKnifeTaking = 3,
        Idle             = 4
    };

    void setLeftKnife(Knife *knife);
    void setRightKnife(Knife *knife);
    Condition getCondition() const;
    int getHungerLevel() const;

public slots:
    void setIsSimulationAllowed(bool value);
    void setIsEatingAllowed(bool value);

private:
    int id;
    int hungerLevelPercent; // Уровень голода в процентах; 0 - сыт, 100 - голоден
    bool isHungry;
    bool isSimulationAllowed; // Симуляция запущена
    bool isEatingAllowed; // Разрешение от алгоритма о возможности приема пищи
    QString name;
    Knife* leftKnife;
    Knife* rightKnife;
    Condition condition;

    static const int updateTimeSecs = 2; // Время одного цикла приема пищи/рассказа истории
    static const int knifeTakingTimeSecs = updateTimeSecs / 2; // Допустим время на взятие ножа в 2 раза меньше времени цикла
    static const int hungerIncreaseSpeed = 5; // Скорость учеличения чувства голода при рассказе истории за цикл
    static const int hungerDecreaseSpeed = 10; // Скорость насыщения за цикл

    void run();
    void updateHungerLevel();

signals:
    void stateChanged(int id, int hunger, int condition);
};

#endif // KNIGHT_H
