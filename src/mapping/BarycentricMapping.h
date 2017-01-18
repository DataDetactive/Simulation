#ifndef BARYCENTRICMAPPING_H
#define BARYCENTRICMAPPING_H

#include <state/State.h>
#include <core/BaseObject.h>
#include <dataStructures.h>
#include <vector>
#include <mapping/Mapping.h>
#include <topology/Topology.h>

class BarycentricMapping : public Mapping {
public:

    void init();

    void apply();

    void applyJT(TVecId f);

    void draw(DisplayFlag flag);

protected:
    Topology * m_topology;
    std::vector<int> m_map_i;
    std::vector<TVec4> m_map_f;

    int computeBaryCoords(TVec3 d,const std::vector<TTetra> & tetras,const std::vector<TVec3> & in,TVec4 & map_f);

};
#endif
