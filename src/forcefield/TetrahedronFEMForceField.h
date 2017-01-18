#ifndef TETRAHEDRONFEMFORCEFIELD_H
#define TETRAHEDRONFEMFORCEFIELD_H

#include <forcefield/ForceField.h>
#include <state/State.h>
#include <topology/Topology.h>

class TetrahedronFEMForceField : public ForceField {
public:

    Data<float> d_youngModulus;
    Data<float> d_poissonRatio;
    Data<std::string> d_method;

    TetrahedronFEMForceField();

    virtual void init() = 0;

    virtual void addForce(TVecId f) = 0;

    virtual void addDForce(MechanicalParams params,TVecId dx,TVecId df) = 0;

    void draw(DisplayFlag flag);

protected:
    std::vector<TMat3x3> m_iR;
    std::vector<TMat3x3> m_R;

};


#endif
