#include <topology/Topology.h>
#include <vector>
#include <string>
#include <core/BaseObject.h>
#include <dataStructures.h>
#include <state/State.h>

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

DECLARE_ALIAS(TetrahedronSetTopologyContainer, Topology)
DECLARE_ALIAS(TriangleSetTopologyContainer, Topology)

