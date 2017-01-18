#ifndef LOADER_H
#define LOADER_H

#include <vector>
#include <string>
#include <core/BaseObject.h>
#include <dataStructures.h>
#include <topology/Topology.h>
#include <state/State.h>

class Loader : public BaseObject {
public:
    Data<std::string> d_filename;
    Data<TVec3> d_scale3d;

    Loader();

    void init();

protected:
    virtual void load(Topology * topo, State * state, const char * fn) = 0;

};

#endif
