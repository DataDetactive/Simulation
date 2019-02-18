#include "OglShader.h"
#include <stdio.h>
#include <visual/OglTexture.h>
#include <loader/Loader.h>
#include <core/Visitor.h>
#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <core/Context.h>

OglShader::OglShader()
: d_shaderVertex("vertFilename","bumpVertexShader.glsl",this)
, d_shaderFragment("fragFilename","bumpFragmentShader.glsl",this)
{}

void OglShader::init() {
    shaders_reload();
}



std::string OglShader::loadTextFile(const std::string& filename)
{
    // Open the file passed in
    std::ifstream fin(getFullPath(filename.c_str()).c_str());

    // Make sure we opened the file correctly
    if(!fin) {
        std::cerr << "Error OglModel " << this->getName() << " cannot find shader " << filename << std::endl;
        return std::string("");
    }

    std::string strLine = "";
    std::string strText = "";

    // Go through and store each line in the text file within a "string" object
    while(std::getline(fin, strLine))
    {
        strText += "\n" + strLine;
    }

    // Close our file
    fin.close();

    // Return the text file's data
    return strText;
}

GLint OglShader::loadShader(GLint target, const std::string& filename)
{
    std::string source = loadTextFile(filename);
    if (source.empty()) return -1;

    GLint shader = glCreateShaderObjectARB(target);
    const char* src = source.c_str();
    glShaderSourceARB(shader, 1, &src, NULL);
    glCompileShaderARB(shader);

    GLint compiled = 0, length = 0, laux = 0;
    glGetObjectParameterivARB(shader, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
    if (!compiled) std::cerr << "ERROR: Compilation of "<<filename<<" shader failed:"<<std::endl;
//    else std::cout << "Compilation of "<<filename<<" shader OK" << std::endl;

    glGetObjectParameterivARB(shader, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);
    if (length > 1)
    {
        GLcharARB *logString = (GLcharARB *)malloc((length+1) * sizeof(GLcharARB));
        glGetInfoLogARB(shader, length, &laux, logString);
        std::cerr << logString << std::endl;
        free(logString);
    }

    return shader;
}

void OglShader::shaders_reload()
{
//    if (!d_use_shaders.getValue())
//    {
//        std::cout << "Shaders disabled" << std::endl;
//        return;
//    }

//    if (m_textureNormal == 0)
//    {
//        std::cout << "No normalmap texture" << std::endl;
//        return;
//    }


    GLint oldShaderVertex = m_shaderVertex;
    GLint oldShaderFragment = m_shaderFragment;
    GLint oldShaderProgram = m_shaderProgram;
    m_shaderVertex = loadShader(GL_VERTEX_SHADER_ARB, getFullPath(d_shaderVertex.getValue().c_str()));
    m_shaderFragment = loadShader(GL_FRAGMENT_SHADER_ARB, getFullPath(d_shaderFragment.getValue().c_str()));
    if (m_shaderVertex == -1 || m_shaderFragment == -1) return;

    if (m_shaderVertex && m_shaderFragment)
    {
        m_shaderProgram = glCreateProgramObjectARB();
        /* use program object */
        glAttachObjectARB(m_shaderProgram, m_shaderVertex);
        glAttachObjectARB(m_shaderProgram, m_shaderFragment);

        /* link */
        glLinkProgramARB(m_shaderProgram);
        GLint status = 0, length = 0, laux = 0;
        glGetObjectParameterivARB(m_shaderProgram, GL_OBJECT_LINK_STATUS_ARB, &status);
        if (!status) std::cerr << "ERROR: Link of shaders failed:"<<std::endl;
//        else std::cout << "Link of shaders OK" << std::endl;
        glGetObjectParameterivARB(m_shaderProgram, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);
        if (length > 1)
        {
            GLcharARB *logString = (GLcharARB *)malloc((length+1) * sizeof(GLcharARB));
            glGetInfoLogARB(m_shaderProgram, length, &laux, logString);
            std::cerr << logString << std::endl;
            free(logString);
        }
        if (!status) m_shaderProgram = 0;
    }
    if (m_shaderProgram) // load successfull
    {
        if (oldShaderVertex) glDeleteObjectARB(oldShaderVertex);
        if (oldShaderFragment) glDeleteObjectARB(oldShaderFragment);
        if (oldShaderProgram) glDeleteObjectARB(oldShaderProgram);
        glUseProgramObjectARB(m_shaderProgram);
        m_shaderTangentAttrib = glGetAttribLocationARB(m_shaderProgram,"tangent");
        m_shaderColorMap = glGetUniformLocationARB(m_shaderProgram,"planarTexture");
        m_shaderNormalMap = glGetUniformLocationARB(m_shaderProgram,"normalMap");
        glUniform1iARB(m_shaderColorMap,0);
        glUniform1iARB(m_shaderNormalMap,1);
        glUseProgramObjectARB(0);
    }
    else // keep previous
    {
        if (m_shaderVertex) glDeleteObjectARB(m_shaderVertex);
        m_shaderVertex = oldShaderVertex;
        if (m_shaderFragment) glDeleteObjectARB(m_shaderFragment);
        m_shaderFragment = oldShaderFragment;
        if (m_shaderProgram) glDeleteObjectARB(m_shaderProgram);
        m_shaderProgram = oldShaderProgram;
    }
}




DECLARE_CLASS(OglShader)
