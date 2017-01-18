#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <core/BaseObject.h>
#include <core/Visitor.h>

class Constraint : public BaseObject {
public:

    virtual void applyConstraint(TVecId f) = 0;

};

class ApplyConstraintVisitor : public Visitor {
public:

    ApplyConstraintVisitor(TVecId f) : Visitor(true) {
        m_f = f;
    }

    bool processObject(BaseObject * o) {
        if (dynamic_cast<Constraint *>(o)) {
            ((Constraint *) o)->applyConstraint(m_f);
        }

        return true;
    }

    TVecId m_f;
};

#endif
