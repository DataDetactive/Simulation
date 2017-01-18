#include "MeshVTKLoader.h"
#include <iostream>
#include <fstream>

MeshVTKLoader::MeshVTKLoader()
: d_flipTetra("flipTetra",false,this) {}


//// TODO : implement this function
//bool MeshVTKLoader::read_vtk_nodes(State * state,std::ifstream & in) {}

//// TODO : implement this function
//bool MeshVTKLoader::read_vtk_elements(Topology * topo,std::ifstream & in) {}

bool MeshVTKLoader::read_vtk_texture(State * state,Topology * topo,std::ifstream & in) {
    std::string line;
    getline( in, line );

    std::vector<TTexCoord> & textures= topo->getTextures();

    for (unsigned int i=0;i<state->size();++i) {
        double t1,t2;

        in >> t1 >> t2;
        TTexCoord tex(t1,t2);

        textures.push_back(tex);
    }

    return true;
}


void MeshVTKLoader::load(Topology * topo,State * state, const char * filename) {

    std::ifstream in(filename);

    if (!in)
    {
        std::cerr << "Cannot open file " << filename << std::endl;
        return;
    }

//    std::cout << "Reading file " << filename << std::endl;
    std::string type;

    while (in >> type) {
        if (type == "POINTS") {
            if (! read_vtk_node(state,in)) return ;
        } else if (type == "CELLS") {
            if (! read_vtk_element(topo,in)) return ;
        } else if (type == "POLYGONS") {
            if (! read_vtk_element(topo,in)) return ;
        } else if (type == "TEXTURE_COORDINATES") {
            if (! read_vtk_texture(state,topo,in)) return ;
        }
    }

    if (d_printLog.getValue())
        std::cout << this->getName() << " loaded " << state->size() << " points and " << topo->getTetras().size() << " tetras and " << topo->getTriangles().size() << " triangles and " << topo->getTextures().size() << " textures" << std::endl;

    in.close();
}


DECLARE_CLASS(MeshVTKLoader)

