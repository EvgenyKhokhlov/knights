#ifndef KNIGHTANIMATION_H
#define KNIGHTANIMATION_H

#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>
#include <QObject>
#include <QDebug>
#include "GraphicScene/animationpool.h"

class KnightAnimation : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(int currentFrame READ getCurrentFrame WRITE setCurrentFrame)
public:
    explicit KnightAnimation(QObject *parent = 0);

    void startAnimation();
    void stopAnimation();
    void setDefaultRotation(int value);
    void changeAnimation(int animationID);

private slots:
    void animationFinished();

private:
    QMap<int, QVector<QPixmap*> > frames;
    QVector<QPixmap*> currentFrames;
    QPropertyAnimation *animation;
    int currentFrameIndex;
    int defaultRotation;
    int nextAnimationID;
    bool isAnimationEnabled;

    int getCurrentFrame() const;
    void setCurrentFrame(int frameIndex);
    bool setAnimationSettings(int stateID, bool isLooped, bool isReverse = false);
    void setDefaultState();

    static const int speedPerFrameMSecs = 100;

    enum State{
        Eating,
        StoryTelling,
        TakingLeftKnife,
        TakingRightKnife
    };

    State currentState;

    static const int mirroredFramesOffsetX = -20;
    static const int eatingFramesRotation = 25;
};

#endif // KNIGHTANIMATION_H
