#include <mapping/BarycentricMapping.h>
#include <stdio.h>
#include <loader/Loader.h>
#include <core/Visitor.h>
#include <state/State.h>
#include <GL/gl.h>

void BarycentricMapping::init() {
    Mapping::init();

    if (m_stateFrom == NULL) return;
    if (m_stateTo == NULL) return;

    // we need the topology of the mstateFrom, we exectute a visitor from the context of the mstateFrom
    m_topology = FindVisitor<Topology>::find(m_stateFrom->getContext());
    if (m_topology == NULL) {
        std::cerr << "Error BarycentricMapping " << this->getName() << " cannot find topology from mstate " << m_stateFrom->getName() << std::endl;
        return;
    }

    const std::vector<TTetra> & tetras = m_topology->getTetras();
    const std::vector<TVec3> & in = m_stateFrom->get(VecID::position);
    const std::vector<TVec3> & out = m_stateTo->get(VecID::position);

    //For all points in out (i.e. the mstate dest):
    // map_i store the id of the tetra in which out[i] is located
    // map_f store the barycentric coordinates of the point inside the tetra
    m_map_i.resize(out.size());
    m_map_f.resize(out.size());

    for (unsigned i=0;i<out.size();i++) {
        m_map_i[i] = computeBaryCoords(out[i],tetras,in,m_map_f[i]);
    }
}

//// TODO : implement this function
int BarycentricMapping::computeBaryCoords(TVec3 out,const std::vector<TTetra> & tetras,const std::vector<TVec3> & in,TVec4 & map_f) {
    double min_dist = std::numeric_limits<double>::max();
    int min_id = 0;
    map_f = TVec4(1.0,0.0,0.0,0.0);

    //we seach for the tetra containing the point out
    for (unsigned i=0;i<tetras.size();i++) {
        TVec3 a = in[tetras[i][1]] - in[tetras[i][0]];
        TVec3 b = in[tetras[i][2]] - in[tetras[i][0]];
        TVec3 c = in[tetras[i][3]] - in[tetras[i][0]];

        TVec3 e = out - in[tetras[i][0]];

        // volume of the tetra Vd=(a,b,c,d) and 3 tetras Va=(e,b,c,d) Vb=(a,e,c,d) Vc=(a,b,e,d)
        double Vd = 1.0/6.0 * dot(a,cross(b,c));
        double Va = 1.0/6.0 * dot(e,cross(b,c));
        double Vb = 1.0/6.0 * dot(a,cross(e,c));
        double Vc = 1.0/6.0 * dot(a,cross(b,e));

        double ca = Va/Vd;
        double cb = Vb/Vd;
        double cc = Vc/Vd;
        double cd = 1.0 - (ca + cb + cc);

        //accumulate the distance to the tetrahedra (in terms of barycentric coordinate)
        double dist = 0.0;
        if (ca < 0) dist += -ca;
        else if (ca > 1) dist += ca-1;

        if (cb < 0) dist += -cb;
        else if (cb > 1) dist += cb-1;

        if (cc < 0) dist += -cc;
        else if (cc > 1) dist += cc-1;

        if (cd < 0) dist += -cd;
        else if (cd > 1) dist += cd-1;

        // if dist == 0 it means we are inside the tetra.
        if (dist == 0.0) {
            map_f = TVec4(cd,ca,cb,cc);
            return i;
        } else if (dist < min_dist) {
            map_f = TVec4(cd,ca,cb,cc);
            min_dist = dist;
            min_id = i;
        }
    }

    return min_id;
}

//// TODO : implement this function
//void BarycentricMapping::apply() {}

//// TODO : implement this function
//void BarycentricMapping::applyJT(TVecId vf) {}

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
