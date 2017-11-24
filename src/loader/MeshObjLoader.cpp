#include <loader/MeshObjLoader.h>
#include <iostream>
#include <fstream>

bool MeshObjLoader::read_obj(Topology * topo,State * state, std::ifstream & in) {
    std::vector<TVec3> & m_points = state->get(VecID::restPosition);
    std::vector<TTriangle> & m_triangles = topo->getTriangles();
    std::vector<TTexCoord> & m_textures = topo->getTextures();


//    m_positions.clear();
    m_triangles.clear();
    m_textures.clear();

//    std::vector<TVec3> & points = m_positions;
    std::vector<TTriangle> & triangles = m_triangles;
    std::vector<TTexCoord> * texcoords = &m_textures;

    int nbp = 0;
    int nbt = 0;
    int nbvt = 0;

    std::string line;
    std::vector<TTexCoord>intexcoords;

    std::vector<int> pointTexCoordId;
    // for points with multiple tex coords, we use a temporary buffer and negative ids in triangles
    std::vector<TVec3> tmppoints;
    std::vector<TTexCoord> tmptexcoords;
    std::map< std::pair<int,TTexCoord>, int > vt2point;

    while( std::getline(in,line) )
    {
        if (line.empty()) continue;
        std::istringstream values(line);
        std::string token;
        values >> token;
        if (token == "#")
        {
            /* comment */
        }
        else if (token == "v")
        {
            /* vertex */
            TVec3 pos;
            values >> pos[0] >> pos[1] >> pos[2];
            pos[0] *= d_scale3d.getValue()[0];
            pos[1] *= d_scale3d.getValue()[1];
            pos[2] *= d_scale3d.getValue()[2];

            state->addPoint(pos);
            pointTexCoordId.push_back(-1);
            ++nbp;
        }
        else if (token == "vn")
        {
            /* normal */
            double x,y,z;
            values >> x >> y >> z;
        }
        else if (token == "vt")
        {
            /* texcoord */
            if (texcoords)
            {
                intexcoords.resize(nbvt+1);
                values >> intexcoords[nbvt][0] >> intexcoords[nbvt][1];
                ++nbvt;
            }
            else
            {
                double u,v;
                values >> u >> v;
            }
        }
        else if (token == "mtllib")
        {
            while (!values.eof())
            {
                std::string mtlfile;
                values >> mtlfile;
                //readMTL(mtlfile.c_str());
            }
        }
        else if (token == "usemtl" || token == "g")
        {
            /* group & material */
        }
        else if (token == "f")
        {
            /* face */
            int fp[3];
            int np = 0;
            int vtn[3];
            while (!values.eof())
            {
                std::string face;
                values >> face;
                if (face.empty()) continue;
                for (int j = 0; j < 3; j++)
                {
                    vtn[j] = 0;
                    std::string::size_type pos = face.find('/');
                    std::string tmp = face.substr(0, pos);
                    if (pos == std::string::npos)
                        face = "";
                    else
                    {
                        face = face.substr(pos + 1);
                    }

                    if (!tmp.empty())
                        vtn[j] = atoi(tmp.c_str()) - 1; // -1 because the indices begin at 1 and C vectors begin at 0
                }
                fp[np] = vtn[0];
                if (texcoords && nbvt > 0) // we need to check for different texture coordinate
                {
                    if (pointTexCoordId[vtn[0]] == -1)
                    {   // first use of this vertex -> store the texture coordinate index
                        pointTexCoordId[vtn[0]] = vtn[1];
                        texcoords->resize(nbp);
                        (*texcoords)[vtn[0]] = intexcoords[vtn[1]];
                    }
                    else if (intexcoords[pointTexCoordId[vtn[0]]] != intexcoords[vtn[1]])
                    {   // different texture coordinate -> use a temporary point
                        int& index = vt2point[std::make_pair(vtn[0],intexcoords[vtn[1]])];
                        if (index == 0) // create new point
                        {
                            index = -1-tmppoints.size();
                            tmppoints.push_back(m_points[vtn[0]]);
                            tmptexcoords.push_back(intexcoords[vtn[1]]);
                        }
                        fp[np] = index;
                    }
                }
                ++np;
                if (np == 3)
                { // new triangle
                    triangles.resize(nbt+1);
                    triangles[nbt][0] = fp[0];
                    triangles[nbt][1] = fp[1];
                    triangles[nbt][2] = fp[2];
                    ++nbt;
                    fp[1] = fp[2]; // if the OBJ facet has more than 3 vertices, we create several triangles in a fan structure around the first vertex
                    --np;
                }
            }
        }
        else
        {
            // std::cerr << "readObj : Unknown token for line " << line << std::endl;
        }
    }
//    std::cout << nbp << " points" << std::endl;
//    std::cout << nbt << " triangles" << std::endl;
//    std::cout << texcoords->size() << " texcoords" << std::endl;


    if (texcoords && !tmppoints.empty())
    {
        int nbtmp = tmppoints.size();
        texcoords->resize(nbp + nbtmp);
        for (int i=0; i<nbtmp; ++i)
        {
            state->addPoint(tmppoints[i]);
            (*texcoords)[nbp + i] = tmptexcoords[i];
        }
        for (int i=0; i<nbt; ++i)
        {
            for (int j=0; j<3; ++j)
            {
                int p = (int)triangles[i][j];
                if (p < 0) // temporary point
                    triangles[i][j] = nbp + (-p-1);
            }
        }
        nbp += nbtmp;
//        std::cout << nbp << " output points (for texture mapping)" << std::endl;
    }

    in.close();

    return true;
}


void MeshObjLoader::load(Topology * topo,State * state,const char * filename) {
    std::ifstream in(filename);
    if (!in)
    {
        std::cerr << "Cannot open file " << filename << std::endl;
        return;
    }

//    std::cout << "Reading file " << filename << std::endl;

    read_obj(topo,state,in);

    in.close();
}



DECLARE_CLASS(MeshObjLoader)

