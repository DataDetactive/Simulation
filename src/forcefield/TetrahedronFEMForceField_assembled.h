#ifndef TETRAHEDRONFEMFORCEFIELDASSEMBLED_H
#define TETRAHEDRONFEMFORCEFIELDASSEMBLED_H

#include <forcefield/TetrahedronFEMForceField.h>


class TetrahedronFEMForceField_assembled : public TetrahedronFEMForceField {
public :

    void init();

    void addForce(TVecId f);

    void addDForce(MechanicalParams params,TVecId dx,TVecId df);

    std::vector<TMat12x12> m_vecKe;
};

#endif
