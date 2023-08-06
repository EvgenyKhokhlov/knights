#ifndef KNIGHTSDATA_H
#define KNIGHTSDATA_H

#include <QObject>

#include "knight.h"

class KnightsData : public QObject
{
    Q_OBJECT
public:
    explicit KnightsData(QObject *parent = 0);

    QVector<QString> getNames() const;
    QVector<int> getHunger() const;
    QVector<int> getCondition() const;
    QVector<int> getMealsNumber() const;
    QVector<int> getStoriesToldNumber() const;
    QVector<bool> getLeftKnifeInHand() const;
    QVector<bool> getRightKnifeInHand() const;
    int getKnightsCount() const;

private:
    QVector<int> knightsHunger;
    QVector<int> knightsCondition;
    // Для корректного подсчета количества рассказанных историй запоминаем последнее состояние приема пищи или рассказа
    QVector<int> tempCondition;
    QVector<int> mealsNumber;
    QVector<int> storiesToldNumber;
    QVector<QString> knightsNames;
    QVector<bool> leftKnifeInHand;
    QVector<bool> rightKnifeInHand;

signals:
    void knightState(int id, int state);

public slots:
    void knightStateChanged(int id, int knightsHunger, int knightsCondition);
};

#endif // KNIGHTSDATA_H
