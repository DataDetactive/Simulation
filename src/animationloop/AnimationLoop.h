#ifndef ANIMATIONLOOP_H
#define ANIMATIONLOOP_H

#include <core/BaseObject.h>
#include <dataStructures.h>
#include <vector>

class AnimationLoop : public BaseObject {
public:

    AnimationLoop() {
        m_time = 0.0;
    }

    virtual void step() = 0;

protected:
    double m_time;

};
#endif
