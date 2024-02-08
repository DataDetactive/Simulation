#ifndef DEFAULTANIMATIONLOOP_H
#define DEFAULTANIMATIONLOOP_H

#include <animationloop/AnimationLoop.h>
#include <core/Visitor.h>
#include<integrator/Integrator.h>
#include<mapping/Mapping.h>

#include <animationloop/AnimationLoop.h>

class DefaultAnimationLoop : public AnimationLoop {
public:

    void step(double time);

};
// create visitors allowing the function to be performed each time it is needed.
class ApplyIntegratorVisitor : public Visitor{
    public:
    ApplyIntegratorVisitor(double t) : Visitor(true) {
            m_t = t;
        }
    // check if it is an integrator
    bool processObject(BaseObject * o) {
            if (dynamic_cast<Integrator *>(o)) {
                ((Integrator *) o)->step(m_t);
            }

            return true;
        }
    double m_t;
};


class ApplyMappingVisitor : public Visitor {
public:

    ApplyMappingVisitor() : Visitor(true) {

    }

    bool processObject(BaseObject * o) {
        if (dynamic_cast<Mapping *>(o)) {
            ((Mapping *) o)->apply();
        }

        return true;
    }


};
#endif
