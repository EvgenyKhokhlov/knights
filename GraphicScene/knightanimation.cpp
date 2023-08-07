#include "knightanimation.h"


KnightAnimation::KnightAnimation(QObject *parent) : QObject(parent), QGraphicsPixmapItem()
{           
    nextAnimationID = -1;

    frames.insert(StoryTelling, AnimationPool::getAnimation(0));

    // Собираем спрайты для анимации взятия правого ножа
    QVector<QPixmap*> takingRightKnifeSprites = AnimationPool::getAnimation(1);
    takingRightKnifeSprites.append(takingRightKnifeSprites.at(4));
    takingRightKnifeSprites.append(takingRightKnifeSprites.at(3));
    takingRightKnifeSprites.remove(0, 3);
    takingRightKnifeSprites.remove(2, 6);
    frames.insert(TakingRightKnife, takingRightKnifeSprites);

    // Собираем спрайты для анимации взятия левого ножа
    QVector<QPixmap*> takingLeftKnifeSprites = AnimationPool::getAnimation(2);
    takingLeftKnifeSprites.append(takingLeftKnifeSprites.at(4));
    takingLeftKnifeSprites.append(takingLeftKnifeSprites.at(3));
    takingLeftKnifeSprites.remove(0, 3);
    takingLeftKnifeSprites.remove(2, 6);
    frames.insert(TakingLeftKnife, takingLeftKnifeSprites);

    // Собираем спрайты для анимации приема пищи
    QVector<QPixmap*> eatingSprites = AnimationPool::getAnimation(1);
    eatingSprites.remove(0, 6);
    eatingSprites.remove(5, 7);
    frames.insert(Eating, eatingSprites);

    animation = new QPropertyAnimation;
    animation->setTargetObject(this);
    animation->setPropertyName("currentFrame");
    animation->setStartValue(0);
    connect(animation, SIGNAL(finished()), this, SLOT(animationFinished()));

    setDefaultState();
    // Задаем центральную точку
    setTransformOriginPoint(70, 85);
}

int KnightAnimation::getCurrentFrame() const
{
    return currentFrameIndex;
}

void KnightAnimation::setCurrentFrame(int frameIndex)
{
    if (frameIndex != currentFrameIndex && frameIndex >= 0 && frameIndex < currentFrames.size())
    {
        if(currentState == TakingLeftKnife)
            // Для зеркального набора кадров задаем смещение относителньо центра
            setOffset(mirroredFramesOffsetX, 0);
        else
            setOffset(0, 0);

        if(currentState == Eating)
            // Разворачиваем кадры чтобы персонаж смотрел прямо во время приема пищи
            setRotation(defaultRotation + eatingFramesRotation);
        else
            setRotation(defaultRotation);

        currentFrameIndex = frameIndex;
        setPixmap(*(currentFrames.at(currentFrameIndex)));
    }
}

bool KnightAnimation::setAnimationSettings(int stateID, bool isLooped, bool isReverse)
{
    if(!frames.contains(stateID)){
        qWarning() << "Missing animation No: " << stateID;
        return false;
    }

    currentFrames = frames[stateID];
    currentState = static_cast<State>(stateID);

    animation->setEndValue(currentFrames.size());
    animation->setDuration(currentFrames.size() * speedPerFrameMSecs);

    if(isLooped)
        animation->setLoopCount(-1);
    else
        animation->setLoopCount(1);

    if(isReverse)
        animation->setDirection(QAbstractAnimation::Backward);
    else
        animation->setDirection(QAbstractAnimation::Forward);

    return true;
}

void KnightAnimation::animationFinished()
{
    if(!isAnimationEnabled) return;

    // Если закончила проигрываться анимация взятия левого ножа
    if(currentState == TakingLeftKnife){
        // И в очереди есть анимация взятия правого ножа, проигрываем ее
        if(nextAnimationID == TakingRightKnife){
            nextAnimationID = -1; // Очищаем очередь
            setAnimationSettings(TakingRightKnife, false);
            animation->start();
            return;
        }
        // И в очереди есть эта же анимация, значит ложим нож на место - проигрываем ее в обратном порядке
        if(nextAnimationID == TakingLeftKnife && animation->direction() == QAbstractAnimation::Forward){
            nextAnimationID = -1; // Очищаем очередь
            setAnimationSettings(TakingLeftKnife, false, true);
            animation->start();
            return;
        }
        // Иначе взять нож не получилось - запускаем анимцию рассказа истории
        setAnimationSettings(StoryTelling, true);
        animation->start();
        return;
    }
    // Если закончила проигрываться анимация взятия правого ножа
    if(currentState == TakingRightKnife){
        // и в очереди положить левый нож - запускает в обратном порядке
        if(nextAnimationID == TakingLeftKnife){
            nextAnimationID = -1; // Очищаем очередь
            setAnimationSettings(TakingLeftKnife, false, true);
            animation->start();
            return;
        }
        // Запускаем анимацию приема пищи
        setAnimationSettings(Eating, true);
        animation->start();
        return;
    }
}

void KnightAnimation::changeAnimation(int animationID)
{
    // Если пришел сигнал о взятии левого ножа - останавливаем текущую анимацию и запускаем анимацию взятия левого ножа
    if(animationID == TakingLeftKnife){
        animation->stop();
        setAnimationSettings(TakingLeftKnife, false);
        animation->start();
        return;
    }
    // Если пришел сигнал о взятии правого ножа
    if(animationID == TakingRightKnife){
        if(currentState != TakingLeftKnife) return; // Если последняя анимация не взятие левого ножа - выходим

        // И при этом все еще проигрывается анимация взятия левого, добавляем в очередь анимацию взятия правого ножа
        if(animation->state() != QAbstractAnimation::Stopped){
            nextAnimationID = TakingRightKnife;
        }else{ // иначе запускаем сразу
            setAnimationSettings(TakingRightKnife, false);
            animation->start();
        }
        return;
    }
    // Если пришел сигнал о рассказе истории пока была проиграна анимация взятия левого ножа -
    // значит правый взять не удалось и нужно положить левый на место, запускаем анимацию в обратном порядке
    if(animationID == StoryTelling && currentState == TakingLeftKnife){
        // Если анимация не закончилась - добавляем в очередь
        if(animation->state() != QAbstractAnimation::Stopped){
            nextAnimationID = TakingLeftKnife;
        }else{ // иначе проигрываем сразу
            setAnimationSettings(TakingLeftKnife, false, true);
            animation->start();
        }
        return;
    }
    // Закончили есть - ложим правый нож
    if(animationID == StoryTelling && currentState == Eating){
        animation->stop();
        setAnimationSettings(TakingRightKnife, false, true);
        nextAnimationID = TakingLeftKnife;
        animation->start();
    }
}

void KnightAnimation::startAnimation()
{
    isAnimationEnabled = true;
    setAnimationSettings(StoryTelling, true);
    animation->start();
}

void KnightAnimation::stopAnimation()
{
    isAnimationEnabled = false;
    animation->stop();
    setDefaultState();
}

void KnightAnimation::setDefaultRotation(int value)
{
    defaultRotation = value;
    setRotation(defaultRotation);
}

void KnightAnimation::setDefaultState()
{
    setPixmap(*frames[StoryTelling].at(0));
    setOffset(0, 0);
    setRotation(defaultRotation);
}
