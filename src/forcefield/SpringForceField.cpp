#include <forcefield/SpringForceField.h>
#include <state/State.h>
#include <stdio.h>
#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <core/Visitor.h>
#include <GL/glew.h>
#include <GL/gl.h>

SpringForceField::SpringForceField()
: d_index("index",-1,this)
, d_force("force",TVec3(),this)
, d_stiffness("stiffness",40.0,this) {}

void SpringForceField::addForce(TVecId f) {
    if (d_index.getValue() == -1) return;

    this->getContext()->getMstate()->get(f)[d_index.getValue()] += d_force.getValue() * d_stiffness.getValue();
}

void SpringForceField::draw(DisplayFlag flag) {
    if (d_index.getValue() == -1) return;

    std::vector<TVec3> & pos = this->getContext()->getMstate()->get(VecID::position);

    TVec3 p1 = pos[d_index.getValue()];
    TVec3 p2 = p1 + d_force.getValue();

    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(0.8f,0.2f,0.2f); glVertex3fv(p1.ptr());
    glColor3f(1.0f,0.6f,0.6f); glVertex3fv(p2.ptr());
    glEnd(); // GL_LINES
    glLineWidth(1);
}

DECLARE_CLASS(SpringForceField)

