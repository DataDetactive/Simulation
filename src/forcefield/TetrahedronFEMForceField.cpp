#include <forcefield/TetrahedronFEMForceField_optim.h>
#include <state/State.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <core/Visitor.h>

TetrahedronFEMForceField::TetrahedronFEMForceField()
: d_youngModulus("youngModulus",100.0,this)
, d_poissonRatio("poissonRatio",0.4,this)
, d_method("method","large",this)
{}

void TetrahedronFEMForceField::draw(DisplayFlag flag) {
    if (! flag.isActive(DisplayFlag::FORCEFIELD)) return;

    const std::vector<TVec3> & x = this->getContext()->getMstate()->get(VecID::position);
    const std::vector<TTetra> & tetrahedra = this->getContext()->getTopology()->getTetras();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glBegin(GL_TRIANGLES);


    for (unsigned int i=0;i<tetrahedra.size();++i)
    {
        TTetra t = tetrahedra[i];
        TReal color = 0;
        TVec3 a = x[t[0]];
        TVec3 b = x[t[1]];
        TVec3 c = x[t[2]];
        TVec3 d = x[t[3]];
        TVec3 center = (a+b+c+d)*(TReal)0.125;
        a = (a+center)*(TReal)0.666667;
        b = (b+center)*(TReal)0.666667;
        c = (c+center)*(TReal)0.666667;
        d = (d+center)*(TReal)0.666667;

        glColor4f(0+color,0,1-color,1);
        glVertex3fv(a.ptr()); glVertex3fv(c.ptr()); glVertex3fv(b.ptr());

        glColor4f(0+color,0.5f-0.5f*color,1-color,1);
        glVertex3fv(b.ptr()); glVertex3fv(c.ptr()); glVertex3fv(d.ptr());

        glColor4f(0+color,1-color,1-color,1);
        glVertex3fv(c.ptr()); glVertex3fv(a.ptr()); glVertex3fv(d.ptr());

        glColor4f(0.5f+0.5f*color,1-color,1-0.5f*color,1);
        glVertex3fv(d.ptr()); glVertex3fv(a.ptr()); glVertex3fv(b.ptr());


        glColor4f(1,0,0,1);
        glVertex3fv(a.ptr()); glVertex3fv(b.ptr()); glVertex3fv(c.ptr());
        glVertex3fv(b.ptr()); glVertex3fv(d.ptr()); glVertex3fv(c.ptr());
        glVertex3fv(c.ptr()); glVertex3fv(d.ptr()); glVertex3fv(a.ptr());
        glVertex3fv(d.ptr()); glVertex3fv(b.ptr()); glVertex3fv(a.ptr());
    }

    glEnd(); // GL_TRIANGLES
    glDisable(GL_CULL_FACE);
}


