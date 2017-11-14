#include <stdio.h>
#include <constraint/BoxConstraint.h>
#include <topology/Topology.h>
#include <state/State.h>
#include <GL/gl.h>
#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <core/Visitor.h>
#include <core/Context.h>

BoxConstraint::BoxConstraint()
: d_box("box",BoundingBox(),this) {}

////// TODO : implement this function
//void BoxConstraint::init() {
//    //get X0, the initial positions (i.e. vecID restPosition)
//    const std::vector<TVec3> & X0 = ...

//    BoundingBox box = d_box.getValue();

//    // first we check that the BBox is correct (min<max)
//    for (int i=0;i<3;i++) {
//        if (box.min[i] > box.max[i]) {
//            double tmp = box.min[i];
//            box.min[i] = box.max[i];
//            box.max[i] = tmp;
//        }
//    }
//    d_box.setValue(box);

//    for all points i in X0
//        if (X0[i] is in the box)
//            add i in m_indice
//}

//// TODO : implement this function
//void BoxConstraint::applyConstraint(TVecId vf) {}

//// TODO : implement this function
//void BoxConstraint::draw(DisplayFlag flag) {
//    //if the flag STATE is not active we don’t anything
//    if (! flag.isActive(DisplayFlag::STATE)) return;

//    // draw the box
//    glBegin(GL_LINES);// start drawing with line
//        glColor3f(0.0f,0.0f,0.0f);

//        glVertex3fv(p0.ptr());glVertex3fv(p1.ptr());
//        glVertex3fv(p0.ptr());glVertex3fv(p2.ptr());
//        ...

//    glEnd();// end drawing with lines
//    glLineWidth(1);

//    // draw the fixed points
//    glColor3f(1.0f,0.4f,0.4f);
//    glPointSize(10);
//    glBegin(GL_POINTS);// start drawing points
//    const std::vector<TVec3> & pos = ...

//    for all points in m_indices
//        p = fixed point i
//            glVertex3fv(p.ptr());

//    glEnd();// end drawing with points
//    glPointSize(1);
//}

DECLARE_CLASS(BoxConstraint)
