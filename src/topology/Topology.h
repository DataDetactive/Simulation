#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include <vector>
#include <string>
#include <core/BaseObject.h>
#include <dataStructures.h>

class Topology : public BaseObject {
public:

    Data<std::string> d_src; // for sofa compatibility

    Topology();

    std::vector<TTetra> & getTetras();

    std::vector<TTriangle> & getTriangles();

    std::vector<TTexCoord> & getTextures();

    std::vector<TVec3> & getNormals();

    std::vector<TVec3> & getTangents();

    void updateNormals();

    void draw(DisplayFlag flag);

protected:
    std::vector<TTetra> m_tetras;
    std::vector<TTriangle> m_triangles;
    std::vector<TTexCoord> m_textures;

};

#endif

