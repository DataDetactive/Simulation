#ifndef ANIMATIONLOOP_H
#define ANIMATIONLOOP_H

#include <core/BaseObject.h>
#include <dataStructures.h>
#include <vector>

class AnimationLoop : public BaseObject {
public:

    virtual void step(double time) = 0;

};
#endif
