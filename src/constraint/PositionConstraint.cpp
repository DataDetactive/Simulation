#include <constraint/PositionConstraint.h>
#include <topology/Topology.h>
#include <state/State.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <core/Visitor.h>
#include <core/Context.h>

PositionConstraint::PositionConstraint()
: d_index("index",-1,this)
, d_position("position",TVec3(),this) {}

void PositionConstraint::applyConstraint(TVecId vf) {
    if (d_index.getValue() == -1) return;

    std::vector<TVec3> & pos = this->getContext()->getMstate()->get(VecID::position);
    std::vector<TVec3> & f = this->getContext()->getMstate()->get(vf);

    f[d_index.getValue()] = TVec3();
    pos[d_index.getValue()] = d_position.getValue();
}

void PositionConstraint::draw(DisplayFlag /*flag*/) {
    if (d_index.getValue() == -1) return;
    if (this->getContext() == NULL) return;

    std::vector<TVec3> & pos = this->getContext()->getMstate()->get(VecID::position);

    TVec3 p1 = pos[d_index.getValue()];

    glColor3f(1.0f,0.4f,0.4f);
    glPointSize(10);
    glBegin(GL_POINTS);
        glVertex3fv(p1.ptr());
    glEnd();
    glPointSize(1);
}

DECLARE_CLASS(PositionConstraint)
