#ifndef MeshGmshLoader_H
#define MeshGmshLoader_H

#include <loader/Loader.h>

class MeshGmshLoader : public Loader {
public :

    Data<bool> d_flipTetra;

    MeshGmshLoader();

private:

    bool read_msh_node(State * topo,std::ifstream & in);

    bool read_msh_element(Topology * topo,std::ifstream & in);

    void load(Topology * topo, State * state,  const char * fn);

};

#endif
