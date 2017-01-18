#include <mapping/BarycentricMapping.h>
#include <stdio.h>
#include <loader/Loader.h>
#include <core/Visitor.h>
#include <state/State.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

void BarycentricMapping::init() {
    Mapping::init();

    if (m_stateFrom == NULL) return;
    if (m_stateTo == NULL) return;

    m_topology = FindVisitor<Topology>().find(m_stateFrom->getContext());
    if (m_topology == NULL) {
        std::cerr << "Error BarycentricMapping " << this->getName() << " cannot find topology from mstate " << m_stateFrom->getName() << std::endl;
        return;
    }

    const std::vector<TTetra> & tetras = m_topology->getTetras();
    const std::vector<TVec3> & in = m_stateFrom->get(VecID::position);
    const std::vector<TVec3> & out = m_stateTo->get(VecID::position);

    m_map_i.resize(out.size());
    m_map_f.resize(out.size());

    for (unsigned i=0;i<out.size();i++) {
        m_map_i[i] = computeBaryCoords(out[i],tetras,in,m_map_f[i]);
    }
}

//// TODO : implement this function
//int BarycentricMapping::computeBaryCoords(TVec3 out,const std::vector<TTetra> & tetras,const std::vector<TVec3> & in,TVec4 & map_f) {}

//// TODO : implement this function
//void BarycentricMapping::apply() {}

//// TODO : implement this function
//void BarycentricMapping::applyJT(TVecId f) {}

void BarycentricMapping::draw(DisplayFlag flag) {
    if (!flag.isActive(DisplayFlag::MAPPING)) return;

    const std::vector<TVec3> & in = m_stateFrom->get(VecID::position);
    std::vector<TVec3> & out = m_stateTo->get(VecID::position);
    const std::vector<TTetra> & tetras = m_topology->getTetras();

    glColor3f(0.0,1.0,0.0);
    glBegin(GL_LINES);
    for (unsigned int i=0;i<out.size();++i)
    {
        TTetra tetra = tetras[m_map_i[i]];

        glVertex3fv(out[i].ptr());glVertex3fv(in[tetra[0]].ptr());
        glVertex3fv(out[i].ptr());glVertex3fv(in[tetra[1]].ptr());
        glVertex3fv(out[i].ptr());glVertex3fv(in[tetra[2]].ptr());
        glVertex3fv(out[i].ptr());glVertex3fv(in[tetra[3]].ptr());
    }
    glEnd();
}


DECLARE_CLASS(BarycentricMapping)
