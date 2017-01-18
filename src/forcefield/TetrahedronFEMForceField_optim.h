#ifndef TETRAHEDRONFEMFORCEFIELDOPTIM_H
#define TETRAHEDRONFEMFORCEFIELDOPTIM_H

#include <forcefield/TetrahedronFEMForceField.h>

class TetrahedronFEMForceField_optim : public TetrahedronFEMForceField {
public :

    void init();

    void addForce(TVecId f);

    void addDForce(MechanicalParams params,TVecId dx,TVecId df);

    TVec3 m_vecDe;
    std::vector<TVec12> m_vecBe;
};

#endif
