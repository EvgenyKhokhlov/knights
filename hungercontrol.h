#ifndef HUNGERCONTROL_H
#define HUNGERCONTROL_H

#include <QObject>

class HungerControl : public QObject
{
    Q_OBJECT
public:
    explicit HungerControl(QVector<int> *hungerLevel, QVector<int> *knightsCondition, QObject *parent = 0);

signals:

public slots:

private:
    QVector<int> *hunger;
    QVector<int> *condition;

    int operateTimeSecs;

private slots:
    void controlHunger();
};

#endif // HUNGERCONTROL_H
