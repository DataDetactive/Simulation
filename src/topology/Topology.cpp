#include <topology/Topology.h>
#include <vector>
#include <string>
#include <core/BaseObject.h>
#include <dataStructures.h>
#include <state/State.h>
#include <GL/gl.h>

Topology::Topology()
: d_src("src","",this)
{

}

std::vector<TTetra> & Topology::getTetras() {
    return m_tetras;
}


std::vector<TTriangle> & Topology::getTriangles() {
    return m_triangles;
}

std::vector<TTexCoord> & Topology::getTextures() {
    return m_textures;
}

void Topology::draw(DisplayFlag flag) {
    if (flag.isActive(DisplayFlag::FORCEFIELD)) {

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

    } else if (flag.isActive(DisplayFlag::STATE)) {
        glColor3f(0.0f,0.0f,0.0f);
        glPointSize(1);
        glBegin(GL_POINTS);
        const std::vector<TVec3> & X = this->getContext()->getMstate()->get(VecID::position);

        for (unsigned i=0;i<X.size();i++) {
            glVertex3fv(X[i].ptr());
        }

        glEnd();
        glPointSize(1);
    }
}

DECLARE_ALIAS(TetrahedronSetTopologyContainer, Topology)
DECLARE_ALIAS(TriangleSetTopologyContainer, Topology)

