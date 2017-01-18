#ifndef MESHOBJLOADER_H
#define MESHOBJLOADER_H

#include <loader/Loader.h>

class MeshObjLoader : public Loader {
private:

    bool read_obj(Topology * topo,State * state, std::ifstream & in);

    void load(Topology * topo, State * state, const char * fn);

};

#endif
