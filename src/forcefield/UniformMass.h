#ifndef UNIFORMMASS_H
#define UNIFORMMASS_H

#include <forcefield/ForceField.h>

class UniformMass : public ForceField {
public:

    Data<float> d_mass;

    UniformMass();

    void addForce(TVecId f);

    void addDForce(MechanicalParams params,TVecId dx,TVecId df);

    void accFromF(TVecId a, TVecId f);

};

class SolveMassVisitor : public Visitor {
public:
    SolveMassVisitor(TVecId a,TVecId f) {
        m_a = a;
        m_f = f;
    }

    bool processObject(BaseObject * o) {
        if (dynamic_cast<UniformMass *>(o)) {
            ((UniformMass *) o)->accFromF(m_a,m_f);
        }

        return true;
    }

    TVecId m_f;
    TVecId m_a;
};

#endif
