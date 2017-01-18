#ifndef MESHVTKLOADER_H
#define MESHVTKLOADER_H

#include <loader/Loader.h>

class MeshVTKLoader : public Loader {
public :

    Data<bool> d_flipTetra;

    MeshVTKLoader();

private:

    bool read_vtk_node(State * state, std::ifstream & in);

    bool read_vtk_texture(State * state,Topology * topo,std::ifstream & in);

    bool read_vtk_element(Topology * topo,std::ifstream & in);

    void load(Topology * topo, State * state, const char * fn);

};

#endif
