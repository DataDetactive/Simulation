#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include <core/BaseObject.h>
#include <loader/Loader.h>
#include <state/State.h>
#include <forcefield/ForceField.h>
#include <forcefield/UniformMass.h>
#include <core/Visitor.h>
#include <constraint/Constraint.h>

class Integrator : public BaseObject {
public:

    virtual void step(double dt) = 0;
};

class IntegratorStepVisitor : public Visitor {
public:
    IntegratorStepVisitor(double d) : Visitor(true) {
        m_dt = d;
    }

    bool processObject(BaseObject *o) {
        if (dynamic_cast<Integrator *>(o)) {
            ((Integrator *) o)->step(m_dt);
        }

        return true;
    }

    double m_dt;
};

#endif
