#ifndef OGLMODEL_H
#define OGLMODEL_H

#include <state/State.h>
#include <core/BaseObject.h>
#include <dataStructures.h>
#include <vector>
#include <topology/Topology.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

class OglModel : public State {
public:

    Data<TVec3> d_color;
//    Data<bool> d_use_shaders;
    Data<std::string> d_shaderVertex;
    Data<std::string> d_shaderFragment;
    Data<std::string> d_material;//for sofa compatibility

    OglModel();

    void init();

    void draw(DisplayFlag flag);

    std::string loadTextFile(const std::string& filename);

    GLint loadShader(GLint target, const std::string& filename);

    void shaders_reload();

    void updateNormals();

protected:

    GLint m_shaderVertex;
    GLint m_shaderFragment;
    GLint m_shaderProgram;
    GLint m_shaderTangentAttrib;
    GLint m_shaderColorMap;
    GLint m_shaderNormalMap;

    std::vector<TVec3> m_normals;
    std::vector<TVec3> m_tangents;
    std::vector<TLine> lines;


};

#endif
