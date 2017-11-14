#ifndef OGLSHADER_H
#define OGLSHADER_H

#include <visual/Visual.h>
#include <core/BaseObject.h>
#include <dataStructures.h>
#include <vector>
#include <topology/Topology.h>

class OglShader : public BaseObject {
public:

    Data<std::string> d_shaderVertex;
    Data<std::string> d_shaderFragment;

    OglShader();

    std::string loadTextFile(const std::string& filename);

    GLint loadShader(GLint target, const std::string& filename);

    void shaders_reload();

    void init();

    virtual GLint getShaderVertex() { return m_shaderVertex; }
    virtual GLint getShaderFragment() { return m_shaderFragment; }
    virtual GLint getShaderProgram() { return m_shaderProgram; }
    virtual GLint getShaderTangentAttrib() { return m_shaderTangentAttrib; }
    virtual GLint getShaderColorMap() { return m_shaderColorMap; }
    virtual GLint getShaderNormalMap() { return m_shaderNormalMap; }

protected:

    GLint m_shaderVertex;
    GLint m_shaderFragment;
    GLint m_shaderProgram;
    GLint m_shaderTangentAttrib;
    GLint m_shaderColorMap;
    GLint m_shaderNormalMap;

};

#endif
