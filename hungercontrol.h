#ifndef HUNGERCONTROL_H
#define HUNGERCONTROL_H

#include <QObject>
#include <QTimer>
#include <QVector>

class HungerControl : public QObject
{
    Q_OBJECT
public:
    explicit HungerControl(QVector<int> *hungerLevel, QObject *parent = 0);

    void startSimulation();
    void stopSimulation();

signals:
    void isEatingAllowed(int knightID, bool value);

private:
    QVector<int> *hunger;
    QTimer *timer;

    static const int operateTimeSecs = 2;
    static const int hungerLevelDifference = 40;

    int nextID(int id);
    int prevID(int id);

private slots:
    void controlHunger();
};

#endif // HUNGERCONTROL_H
