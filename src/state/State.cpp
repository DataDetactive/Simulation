#include <stdio.h>
#include <state/State.h>
#include <topology/Topology.h>
#include <core/Visitor.h>
#include <core/Context.h>
#include <mapping/Mapping.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

State::State() {
    m_size = 0;
    m_mapped = false;
}

//// TODO : implement this function
//void State::vClear(TVecId va) {}

//// TODO : implement this function
//void State::vEqBF(TVecId va,TVecId vb,TReal  h) {}

//// TODO : implement this function
//void State::vPEqBF(TVecId va,TVecId vb,TReal  h) {}

//// TODO : implement this function
//void State::vOp(TVecId vres, TVecId va, TVecId vb, TReal f ) {}

//// TODO : implement this function
//TReal State::vDot(TVecId va, TVecId vb) {}

void State::addPoint(TVec3 p) {
    std::vector<TVec3> & X = this->get(VecID::position);
    std::vector<TVec3> & X0 = this->get(VecID::restPosition);

    X.push_back(p);
    X0.push_back(p);

    m_size = X0.size();
}

unsigned State::size() {
    return m_size;
}

void State::computeBBox(BoundingBox & bbox) {
    std::vector<TVec3> & X = this->get(VecID::position);

    for (unsigned i=0;i<X.size();i++) {
        if (X[i][0] < bbox.min[0]) bbox.min[0] = X[i][0];
        if (X[i][1] < bbox.min[1]) bbox.min[1] = X[i][1];
        if (X[i][2] < bbox.min[2]) bbox.min[2] = X[i][2];

        if (X[i][0] > bbox.max[0]) bbox.max[0] = X[i][0];
        if (X[i][1] > bbox.max[1]) bbox.max[1] = X[i][1];
        if (X[i][2] > bbox.max[2]) bbox.max[2] = X[i][2];
    }
}

std::vector<TVec3> & State::get(TVecId v) {
    std::map<TVecId,std::vector<TVec3> *>::iterator it = m_mapState.find(v);

    if (it == m_mapState.end()) {
        std::vector<TVec3> * res = new std::vector<TVec3>();
        res->resize(size());

        if (d_printLog.getValue()) {
            std::cout << this->getName() << " CREATE " << v << " with size " << size() << std::endl;
        }

        m_mapState[v] = res;
        return *res;
    } else {
        return *it->second;
    }
}

void State::setMapped(bool b) {
    m_mapped = b;
}

bool State::isMechanical() {
    return ! m_mapped;
}

void State::draw(DisplayFlag flag) {
    if (! flag.isActive(DisplayFlag::STATE)) return;

    glColor3f(0.0f,0.0f,0.0f);
    glPointSize(1);
    glBegin(GL_POINTS);
    std::vector<TVec3> & X = get(VecID::position);

    for (unsigned i=0;i<X.size();i++) {
        glVertex3fv(X[i].ptr());
    }

    glEnd();
    glPointSize(1);
}

DECLARE_CLASS(State)
DECLARE_ALIAS(MechanicalObject,State)
