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
        Eating,
        StoryTelling,
        Idle
    };

    void setLeftKnife(Knife *value);
    void setRightKnife(Knife *value);
    Condition getCondition() const;
    int getHungerLevel() const;

    bool isSimulationAllowed; //
    bool isEatingAllowed; // разрешение от алгоритма о возможности приема пищи

private:
    int id;
    int hungerLevelPercent; // 0 - сыт, 100 - голоден
    bool isHungry;
    QString name;
    Knife* leftKnife;
    Knife* rightKnife;
    Condition condition;

    static const int updateTimeSecs = 2;

    void run();

signals:
    void stateChanged(QString message);
};

#endif // KNIGHT_H
