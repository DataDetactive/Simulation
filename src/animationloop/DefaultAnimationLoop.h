#ifndef DEFAULTANIMATIONLOOP_H
#define DEFAULTANIMATIONLOOP_H

#include <animationloop/AnimationLoop.h>

class DefaultAnimationLoop : public AnimationLoop {
public:

    void step(double time);

};
#endif
