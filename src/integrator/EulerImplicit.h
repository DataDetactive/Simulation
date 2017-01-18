#ifndef EULERIMPLICIT_H
#define EULERIMPLICIT_H

#include <integrator/Integrator.h>
#include <loader/Loader.h>
#include <state/State.h>
#include <forcefield/ForceField.h>
#include <forcefield/UniformMass.h>

class EulerImplicit : public Integrator {
public:

    Data<double> d_rayleighMass;
    Data<double> d_rayleighStiffness;
    Data<double> d_vdamping;

    EulerImplicit();

    void step(double dt);

};
#endif
