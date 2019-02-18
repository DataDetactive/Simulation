#include <stdio.h>
#include <constraint/BoxConstraint.h>
#include <topology/Topology.h>
#include <state/State.h>
#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <core/Visitor.h>
#include <core/Context.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

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
//void BoxConstraint::applyConstraint(TVecId vf) {
//    for all points i in the box
//        vf[i] = 0
//}


void BoxConstraint::draw(DisplayFlag flag) {
    if (! flag.isActive(DisplayFlag::STATE)) return;

    State * mstate = this->getContext()->getMstate();
    if (mstate == NULL) return;

    glColor3f(1.0f,0.4f,0.4f);
    glPointSize(10);
    glBegin(GL_POINTS);
    const std::vector<TVec3> & pos = mstate->get(VecID::position);

    for (unsigned i=0;i<m_indices.size();i++) {
        TVec3 p = pos[m_indices[i]];
        glVertex3dv(p.data());
    }
    glEnd();
    glPointSize(1);

    glBegin(GL_LINES);
    glColor3f(0.0f,0.0f,0.0f);
    TVec3 p0 = TVec3(d_box.getValue().min[0],d_box.getValue().min[1],d_box.getValue().min[2]);
    TVec3 p1 = TVec3(d_box.getValue().min[0],d_box.getValue().min[1],d_box.getValue().max[2]);
    TVec3 p2 = TVec3(d_box.getValue().min[0],d_box.getValue().max[1],d_box.getValue().min[2]);
    TVec3 p3 = TVec3(d_box.getValue().min[0],d_box.getValue().max[1],d_box.getValue().max[2]);
    TVec3 p4 = TVec3(d_box.getValue().max[0],d_box.getValue().min[1],d_box.getValue().min[2]);
    TVec3 p5 = TVec3(d_box.getValue().max[0],d_box.getValue().min[1],d_box.getValue().max[2]);
    TVec3 p6 = TVec3(d_box.getValue().max[0],d_box.getValue().max[1],d_box.getValue().min[2]);
    TVec3 p7 = TVec3(d_box.getValue().max[0],d_box.getValue().max[1],d_box.getValue().max[2]);

    glVertex3dv(p0.data());glVertex3dv(p1.data());
    glVertex3dv(p0.data());glVertex3dv(p2.data());
    glVertex3dv(p1.data());glVertex3dv(p3.data());
    glVertex3dv(p2.data());glVertex3dv(p3.data());

    glVertex3dv(p4.data());glVertex3dv(p5.data());
    glVertex3dv(p4.data());glVertex3dv(p6.data());
    glVertex3dv(p5.data());glVertex3dv(p7.data());
    glVertex3dv(p6.data());glVertex3dv(p7.data());

    glVertex3dv(p0.data());glVertex3dv(p4.data());
    glVertex3dv(p1.data());glVertex3dv(p5.data());
    glVertex3dv(p2.data());glVertex3dv(p6.data());
    glVertex3dv(p3.data());glVertex3dv(p7.data());

    glEnd();
    glLineWidth(1);
}

DECLARE_CLASS(BoxConstraint)
