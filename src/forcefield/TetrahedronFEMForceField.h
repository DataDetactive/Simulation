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

    void init();

    void addForce(TVecId f);

    void addDForce(MechanicalParams params,TVecId dx,TVecId df);

protected:
    std::vector<TMat3x3> m_iR;
    std::vector<TMat3x3> m_R;
    std::vector<TMat12x12> m_vecKe;

};


#endif
