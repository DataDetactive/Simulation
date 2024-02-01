#include <stdio.h>
#include <state/State.h>
#include <topology/Topology.h>
#include <core/Visitor.h>
#include <core/Context.h>
#include <mapping/Mapping.h>

State::State() {
    m_size = 0;
    m_mapped = false;
}

//// TODO : implement this function
void State::vClear(TVecId va) {
    std::vector<TVec3> & cleared_vector = this->get(va);
    for (std::vector<TVec3>::size_type j=0;j<cleared_vector.size();j++){
        cleared_vector[j][0]=0;
        cleared_vector[j][1]=0;
        cleared_vector[j][2]=0;

    }
}

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

void State::computeBBox(BoundingBox & min_bbox) {
    std::vector<TVec3> & X = this->get(VecID::position);

    for (unsigned i=0;i<X.size();i++) {
        if (X[i][0] < min_bbox.min[0]) min_bbox.min[0] = X[i][0];
        if (X[i][1] < min_bbox.min[1]) min_bbox.min[1] = X[i][1];
        if (X[i][2] < min_bbox.min[2]) min_bbox.min[2] = X[i][2];

        if (X[i][0] > min_bbox.max[0]) min_bbox.max[0] = X[i][0];
        if (X[i][1] > min_bbox.max[1]) min_bbox.max[1] = X[i][1];
        if (X[i][2] > min_bbox.max[2]) min_bbox.max[2] = X[i][2];
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

DECLARE_CLASS(State)
DECLARE_ALIAS(MechanicalObject,State)
