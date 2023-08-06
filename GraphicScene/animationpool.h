#ifndef ANIMATIONPOOL_H
#define ANIMATIONPOOL_H

#include <QMap>
#include <QPixmap>

class AnimationPool {
public:
    static void load(int id, QString fileAddress, int framesPerColumn, int framesPerRow, int height, int width,
                     bool isMirrored = false);
    static QVector<QPixmap *> getAnimation(int id);
    static int getAnimationCount();

private:
    static QMap<int, QVector<QPixmap*> > animations;

};

#endif // ANIMATIONPOOL_H
