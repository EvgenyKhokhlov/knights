#include "animationpool.h"

QMap<int, QVector<QPixmap*> > AnimationPool::animations;

void AnimationPool::load(int id, QString fileAddress, int framesPerColumn, int framesPerRow, int height, int width,
                         bool isMirrored) {
    QPixmap frames(fileAddress);
    QVector<QPixmap*> animation;
    for (int j = 0; j < framesPerRow; j++){
        for (int i = 0; i < framesPerColumn; i++){
            QPixmap tempImage = QPixmap(frames.copy(i * width, j * height, width, height));
            if(isMirrored){
                tempImage = QPixmap(QPixmap::fromImage(tempImage.toImage().mirrored(true, false)));
            }
            QPixmap *image = new QPixmap(tempImage);
            animation.push_back(image);
        }
    }
    animations[id] = animation;
}

QVector<QPixmap *> AnimationPool::getAnimation(int id)
{
    if (animations.contains(id))
        return animations[id];
    else
        return QVector<QPixmap*>();
}

int AnimationPool::getAnimationCount()
{
    return animations.count();
}


