#include "scene.h"

#include <QDebug>
#include <math.h>

Scene::Scene(int x, int y, int width, int height, QVector<QString> knightsName, QObject *parent) : QGraphicsScene(parent)
{
    setSceneRect(x, y, width - 2, height - 2);

    // Нарезаем SpriteSheets на отдельные картинки и сохраняем
    AnimationPool::load(0, ":/new/prefix1/SpriteSheets/idle.png", 5, 4, 561 / 4, 800 / 5);
    AnimationPool::load(1, ":/new/prefix1/SpriteSheets/attack.png", 6, 3, 421 / 3, 960 / 6);
    AnimationPool::load(2, ":/new/prefix1/SpriteSheets/attack.png", 6, 3, 421 / 3, 960 / 6, true);

    // Размещаем стол
    QGraphicsPixmapItem *table = new QGraphicsPixmapItem;
    table->setPixmap(QPixmap(":/new/prefix1/SpriteSheets/table.png"));
    table->setScale(0.3);
    table->setPos(width / 2 - table->pixmap().width() / 2 * table->scale(),
                  height / 2 - table->pixmap().height() / 2 * table->scale());
    addItem(table);

    double step = 360 / knightsName.count();
    double angleDeg = 0;
    for(int i = 0; i < knightsName.count(); i++){
        double angleRad = angleDeg * M_PI / 180;
        // Размещаем тарелки
        QGraphicsPixmapItem *plate = new QGraphicsPixmapItem;
        plate->setPixmap(QPixmap(":/new/prefix1/SpriteSheets/plate.png"));
        addItem(plate);
        plate->setPos(plateAndKnifePositionRadius * cos(angleRad) + platePositionOffsetX,
                      plateAndKnifePositionRadius * sin(angleRad) + platePositionOffsetY);
        // Размещаем ножи
        QGraphicsPixmapItem *knife = new QGraphicsPixmapItem;
        knifes.append(knife);
        knife->setPixmap(QPixmap(":/new/prefix1/SpriteSheets/knife.png"));
        knife->setTransformOriginPoint(knife->pixmap().width() / 2, knife->pixmap().height() / 2);
        addItem(knife);
        knife->setPos(plateAndKnifePositionRadius * cos(angleRad + knifeAngleOffsetDeg) + knifePositionOffsetX,
                      plateAndKnifePositionRadius * sin(angleRad + knifeAngleOffsetDeg) + knifePositionOffsetY);
        knife->setRotation(angleDeg + knifeAngleRotationOffsetDeg);
        // Размещаем рыцарей
        KnightAnimation *knight = new KnightAnimation;
        knights.append(knight);
        addItem(knight);
        knight->setPos(knightPositionRadius * cos(angleRad) + knightPositionOffsetX,
                       knightPositionRadius * sin(angleRad) + knightPositionOffsetY);
        // Напишем имена рыцарей
        QGraphicsTextItem *name = new QGraphicsTextItem;
        name->setPlainText(knightsName.at(i));
        addItem(name);
        name->setPos(namePositionRadius * cos(angleRad) + namePositionOffsetX,
                     namePositionRadius * sin(angleRad) + namePositionOffsetY);

        angleDeg -= step;
        knight->setDefaultRotation(knightAngleRotationOffsetDeg + angleDeg);

        knightsState.append(-1); // Выставляем исходные состояния рыцарей
    }
}

void Scene::startSimulation()
{
    isSimaltionEnabled = true;
    for(int i = 0; i < knights.count(); i++){
        knights.at(i)->startAnimation();
    }
}

void Scene::stopSimulation()
{
    isSimaltionEnabled = false;
    for(int i = 0; i < knights.count(); i++){
        knights.at(i)->stopAnimation();
        knifes.at(i)->show();
    }
}

void Scene::knightStateChanged(int id, int state)
{
    if(knightsState.at(id) == state || !isSimaltionEnabled) return; // если состояние не поменялось - ничего не делаем
    knightsState[id] = state;

    knights.at(id)->changeAnimation(state);

    if(state == 1){
        if(knightsState.at(prevID(id)) == 1)
            knifes.at(id)->show();
        if(knightsState.at(nextID(id)) == 1)
            knifes.at(nextID(id))->show();
    }
    if(state == 2){
        knifes.at(id)->hide();
    }
    if(state == 3){
        knifes.at(nextID(id))->hide();
    }
}

int Scene::nextID(int id)
{
    if(id >= knifes.count() - 1)
        return 0;
    else
        return id + 1;
}

int Scene::prevID(int id)
{
    if(id - 1 < 0)
        return knifes.count() - 1;
    else
        return id - 1;
}
