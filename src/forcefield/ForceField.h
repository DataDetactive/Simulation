#ifndef FORCEFIELD_H
#define FORCEFIELD_H

#include <core/BaseObject.h>
#include <core/Visitor.h>

class ForceField : public BaseObject {
public:

    virtual void addForce(TVecId f) = 0;

    virtual void addDForce(MechanicalParams params,TVecId /*dx*/,TVecId /*df*/) {}

};

class AddForceVisitor : public Visitor {
public:

    AddForceVisitor(TVecId v) : Visitor(true) {
        m_v = v;
    }

    bool processObject(BaseObject * o) {
        if (dynamic_cast<ForceField *>(o)) {
            ((ForceField *) o)->addForce(m_v);
        }

        return true;
    }

    TVecId m_v;
};

class AddDForceVisitor : public Visitor {
public:

    AddDForceVisitor(MechanicalParams params, TVecId x,TVecId b) : Visitor(true) {
        m_x = x;
        m_b = b;
        m_params = params;
    }

    bool processObject(BaseObject * o) {
        if (dynamic_cast<ForceField *>(o)) {
            ((ForceField *) o)->addDForce(m_params,m_x,m_b);
        }

        return true;
    }

    TVecId m_x;
    TVecId m_b;
    MechanicalParams m_params;
};


#endif
