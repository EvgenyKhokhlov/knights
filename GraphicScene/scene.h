#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QDebug>
#include <math.h>

#include "GraphicScene/knightanimation.h"
#include "GraphicScene/animationpool.h"

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(int x, int y, int width, int height, QVector<QString> knightsName, QObject *parent = 0);

    void startSimulation();
    void stopSimulation();

signals:

public slots:
    void knightStateChanged(int id, int knightsState);

private:
    static const int knightPositionRadius = 150;
    static const int plateAndKnifePositionRadius = 100;
    static const int namePositionRadius = 180;
    static const int knightPositionOffsetX = 178;
    static const int knightPositionOffsetY = 114;
    static const int platePositionOffsetX = 225;
    static const int platePositionOffsetY = 175;
    static const int knifePositionOffsetX = 235;
    static const int knifePositionOffsetY = 185;
    static const int namePositionOffsetX = 230;
    static const int namePositionOffsetY = 190;
    static const int knightAngleRotationOffsetDeg = -20; // Для поворота рыцарей в центр стола
    static const int knifeAngleRotationOffsetDeg = 85; // Для поворота ножей в центр стола
    static const int knifeAngleOffsetDeg = 120; // Для размещения ножей между тарелками

    QVector<KnightAnimation*> knights;
    QVector<QGraphicsPixmapItem*> knifes;
    QVector<int> knightsState;

    bool isSimulationEnabled;

    int nextID(int id);
    int prevID(int id);
};

#endif // SCENE_H
