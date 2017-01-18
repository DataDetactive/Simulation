#ifndef SOLVER_H
#define SOLVER_H

#include <loader/Loader.h>
#include <integrator/Integrator.h>

class Solver : public BaseObject {
public:

    virtual void buildMatrix(MechanicalParams params) = 0;

    virtual void solve(TVecId x, TVecId b) = 0;

};

class SolverVisitor : public Visitor {
public:

    SolverVisitor(MechanicalParams params, TVecId x,TVecId b) : Visitor(true) {
        m_params = params;
        m_x = x;
        m_b = b;
    }

    bool processObject(BaseObject * o) {
        if (Solver * s = dynamic_cast<Solver *>(o)) {
            s->buildMatrix(m_params);
            s->solve(m_x,m_b);
        }

        return true;
    }

    MechanicalParams m_params;
    TVecId m_x;
    TVecId m_b;

};
#endif
