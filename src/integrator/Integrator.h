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

#endif
