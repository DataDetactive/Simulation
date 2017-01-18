#ifndef DEFAULTANIMATIONLOOP_H
#define DEFAULTANIMATIONLOOP_H

#include <animationloop/AnimationLoop.h>

class DefaultAnimationLoop : public AnimationLoop {
public:

    void step();

    void draw(DisplayFlag flag);

};
#endif
