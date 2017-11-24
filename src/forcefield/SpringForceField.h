#ifndef SPRINGFORCEFORCEFIELD_H
#define SPRINGFORCEFORCEFIELD_H

#include <forcefield/ForceField.h>
#include <state/State.h>
#include <topology/Topology.h>

class SpringForceField :  public ForceField {
public:

    Data<int> d_index;
    Data<TVec3> d_force;
    Data<double> d_stiffness;

    SpringForceField();

    void addForce(TVecId f);

    void draw(DisplayFlag);

};


#endif
