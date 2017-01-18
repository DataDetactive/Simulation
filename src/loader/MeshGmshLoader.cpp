#include <loader/MeshGmshLoader.h>
#include <iostream>
#include <fstream>

MeshGmshLoader::MeshGmshLoader()
: d_flipTetra("flipTetra",false,this) {

}

bool MeshGmshLoader::read_msh_node(State * state,std::ifstream & in) {
    unsigned int nbp = 0;
    in >> nbp;

    int idx;
    for (unsigned int i=0;i<nbp;++i)
    {
        TVec3 pos;
        in >> idx >> pos[0] >> pos[1] >> pos[2];
        pos[0] *= d_scale3d.getValue()[0];
        pos[1] *= d_scale3d.getValue()[1];
        pos[2] *= d_scale3d.getValue()[2];

        state->addPoint(pos);
    }

    std::string end;
    in >> end;

    return (end == "$ENDNOD");
}

bool MeshGmshLoader::read_msh_element(Topology * topo,std::ifstream & in) {
    unsigned int nbe = 0;
    in >> nbe;

    std::vector<TTetra> & tetras = topo->getTetras();

    for (unsigned int i=0;i<nbe;++i)
    {
        int idx,t1,t2,t3,t4;
        int a,b,c,d;
        in >> idx >> t1 >> t2 >> t3 >> t4 >> a >> b >> c >> d;

        if (t1==4 && t2==1 && t3==1 && t4==4) {
            TTetra tetra;

            if (d_flipTetra.getValue() == false) {
                tetra[0] = a-1;
                tetra[1] = b-1;
                tetra[2] = c-1;
                tetra[3] = d-1;
            } else {
                tetra[0] = a-1;
                tetra[1] = b-1;
                tetra[2] = d-1;
                tetra[3] = c-1;
            }

            tetras.push_back(tetra);
        } else {
            std::cerr << "Cannot read element type " << t1 << " " << t2 << " "<< t3 << " " << t4 << std::endl;
            continue;
        }

    }

    std::string end;
    in >> end;
    return (end=="$ENDELM");
}


void MeshGmshLoader::load(Topology * topo,State * state, const char * filename) {

    std::ifstream in(filename);

    if (!in)
    {
        std::cerr << "Cannot open file " << filename << std::endl;
        return;
    }

    std::string type;
    while (in >> type) {
        if (type == "$NOD") {
            if (! read_msh_node(state,in)) return ;
        } else if (type == "$ELM") {
            if (! read_msh_element(topo,in)) return ;
        }
    }

    if (d_printLog.getValue()) {
        std::cout << "Loader " << this->getName() << " loaded " << state->size() << " points and " << topo->getTetras().size() << " tetras" << std::endl;
    }

    in.close();
}



DECLARE_CLASS(MeshGmshLoader)

