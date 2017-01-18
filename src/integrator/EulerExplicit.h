#ifndef EULEREXPLICIT_H
#define EULEREXPLICIT_H

#include <integrator/Integrator.h>
#include <loader/Loader.h>
#include <state/State.h>
#include <forcefield/ForceField.h>
#include <forcefield/UniformMass.h>

class EulerExplicit : public Integrator {
public:

    Data<double> d_vdamping;

    EulerExplicit();

    void step(double dt);

};

#endif
