#ifndef STATE_H
#define STATE_H

#include <core/BaseObject.h>
#include <dataStructures.h>
#include <vector>
#include <map>
#include <core/Visitor.h>

class State : public BaseObject {
public:

    State();

    void computeBBox(BoundingBox & bbox);

    std::vector<TVec3> & get(TVecId v);

    void vClear(TVecId a);

    void vEqBF(TVecId a,TVecId b,TReal h);

    void vPEqBF(TVecId a,TVecId b,TReal h);

    void vOp(TVecId res, TVecId a, TVecId b,TReal f );

    TReal vDot(TVecId a, TVecId b);

    void addPoint(TVec3 p);

    unsigned size();

    bool isMechanical();

    void setMapped(bool b);

private:
    std::map<TVecId,std::vector<TVec3> *> m_mapState;
    unsigned m_size;
    bool m_mapped;
};


class VClearVisitor : public Visitor {
public:

    VClearVisitor(TVecId x) : Visitor(true) {
        m_x = x;
    }

    bool processObject(BaseObject * o) {
        if (State * s = dynamic_cast<State *>(o)) {
//            if (! s->isMechanical()) return false;

            s->vClear(m_x);
        }

        return true;
    }

    TVecId m_x;

};


class VEqBFVisitor : public Visitor {
public:

    VEqBFVisitor(TVecId x,TVecId b,TReal h) : Visitor(true) {
        m_x = x;
        m_b = b;
        m_h = h;
    }

    bool processObject(BaseObject * o) {
        if (State * s = dynamic_cast<State *>(o)) {
            if (! s->isMechanical()) return false;

            s->vEqBF(m_x,m_b,m_h);
        }

        return true;
    }

    TVecId m_x;
    TVecId m_b;
    TReal m_h;

};

class VPEqBFVisitor : public Visitor {
public:

    VPEqBFVisitor(TVecId x,TVecId b,TReal h) : Visitor(true) {
        m_x = x;
        m_b = b;
        m_h = h;
    }

    bool processObject(BaseObject * o) {
        if (State * s = dynamic_cast<State *>(o)) {
            if (! s->isMechanical()) return false;

            s->vPEqBF(m_x,m_b,m_h);
        }

        return true;
    }

    TVecId m_x;
    TVecId m_b;
    TReal m_h;

};


class VOpVisitor : public Visitor {
public:
    VOpVisitor(TVecId r,TVecId x,TVecId b,TReal h) : Visitor(true) {
        m_r = r;
        m_x = x;
        m_b = b;
        m_h = h;
    }

    bool processObject(BaseObject * o) {
        if (State * s = dynamic_cast<State *>(o)) {
            if (! s->isMechanical()) return false;

            s->vOp(m_r,m_x,m_b,m_h);
        }

        return true;
    }

    TVecId m_r;
    TVecId m_x;
    TVecId m_b;
    TReal m_h;

};


class VDotVisitor : public Visitor {
public:
    VDotVisitor(TVecId x,TVecId b) : Visitor(true) {
        m_x = x;
        m_b = b;
        m_res = 0.0;
    }

    bool processObject(BaseObject * o) {
        if (State * s = dynamic_cast<State *>(o)) {
            if (! s->isMechanical()) return false;

            m_res += s->vDot(m_x,m_b);
        }

        return true;
    }

    TReal compute(Context * c) {
        execute(c);
        return m_res;
    }

    TVecId m_x;
    TVecId m_b;

    TReal m_res;

};

#endif
