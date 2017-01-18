#include <stdio.h>
#include <state/OglModel.h>
#include <loader/Loader.h>
#include <core/Visitor.h>
#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <fstream>
#include <core/Context.h>
#include <visual/OglTexture.h>
#include <visual/Visual.h>

OglModel::OglModel()
: d_color("color",TVec3(1,1,1),this)
, d_shaderVertex("vertFilename","bumpVertexShader.glsl",this)
, d_shaderFragment("fragFilename","bumpFragmentShader.glsl",this)
, d_material("material","",this)
{}

void OglModel::init() {

    //for wireframe
    std::vector<TTriangle> & triangles = this->getContext()->getTopology()->getTriangles();
    for (unsigned i=0;i<triangles.size();i++) {
        lines.push_back(TLine(triangles[i][0],triangles[i][1]));
        lines.push_back(TLine(triangles[i][0],triangles[i][2]));
        lines.push_back(TLine(triangles[i][1],triangles[i][2]));
    }

    shaders_reload();
}



std::string OglModel::loadTextFile(const std::string& filename)
{
    // Open the file passed in
    std::ifstream fin(filename.c_str());

    // Make sure we opened the file correctly
    if(!fin) {
        std::cerr << "Error OglModel " << this->getName() << " cannot find shader " << filename << std::endl;
        return "";
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

GLint OglModel::loadShader(GLint target, const std::string& filename)
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

void OglModel::shaders_reload()
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

void OglModel::updateNormals() {
    std::vector<TTriangle> & triangles = this->getContext()->getTopology()->getTriangles();
    std::vector<TTexCoord> & textures = this->getContext()->getTopology()->getTextures();
    std::vector<TVec3> & X = this->getContext()->getMstate()->get(VecID::position);

    if (textures.size() == 0) return;

    m_normals.resize(X.size());
    m_tangents.resize(X.size());

    for (unsigned int i=0;i<m_normals.size();++i)
    {
        m_normals[i].clear();
        m_tangents[i].clear();
    }

    for (unsigned int i=0;i<triangles.size();++i)
    {
        TVec3 A = X[triangles[i][0]];
        TVec3 B = X[triangles[i][1]];
        TVec3 C = X[triangles[i][2]];
        B -= A;
        C -= A;
        TVec3 n = cross(B,C);
        n.normalize();
        TReal Au = textures[triangles[i][0]][0];
        TReal Bu = textures[triangles[i][1]][0];
        TReal Cu = textures[triangles[i][2]][0];
        Bu -= Au;
        Cu -= Au;
        TVec3 t = B * Cu - C * Bu;
        t.normalize();
        for (unsigned int j=0;j<3;++j)
        {
            m_normals[triangles[i][j]] += n;
            m_tangents[triangles[i][j]] += t;
        }
    }
    for (unsigned int i=0;i<m_normals.size();++i)
    {
        m_tangents[i] = cross(m_normals[i],m_tangents[i]);
        m_normals[i].normalize();
        m_tangents[i].normalize();
    }
}

void OglModel::draw(DisplayFlag flag) {
    if (!flag.isActive(DisplayFlag::VISUAL)) return;

    std::vector<TVec3> & X = this->get(VecID::position);
    std::vector<TTriangle> & triangles = this->getContext()->getTopology()->getTriangles();
    std::vector<TTexCoord> & textures = this->getContext()->getTopology()->getTextures();
    GLint textureColor = FindTextureIdVisitor(TEXTURE_COLOR).find(this->getContext());
    GLint textureNormal = FindTextureIdVisitor(TEXTURE_NORMAL).find(this->getContext());

    glEnable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);

    glColor3f(d_color.getValue()[0], d_color.getValue()[1], d_color.getValue()[2]);

    glVertexPointer(TVec3::size(),myglType<TVec3::Real>(), 0, &X[0]);

    bool tex = !textures.empty() && (textureColor != -1);
    bool texnormal = (textureNormal != -1) && (m_shaderVertex != -1) && (m_shaderFragment != -1);

    if (tex) {
        updateNormals();

        glNormalPointer(myglType<TReal>(), 0, &m_normals[0]);
        glEnableClientState(GL_NORMAL_ARRAY);

        glTexCoordPointer(TTexCoord::size(), myglType<TTexCoord::Real>(), 0, &textures[0]);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glBindTexture(GL_TEXTURE_2D, textureColor);
        glEnable(GL_TEXTURE_2D);

        if (texnormal) {
            glVertexAttribPointerARB(m_shaderTangentAttrib, TVec3::size(), myglType<TVec3::Real>(), GL_FALSE, 0, &m_tangents[0]);
            glEnableVertexAttribArrayARB(m_shaderTangentAttrib);
            glActiveTextureARB(GL_TEXTURE1_ARB);
            glBindTexture(GL_TEXTURE_2D, textureNormal);
            glEnable(GL_TEXTURE_2D);
            glUseProgramObjectARB(m_shaderProgram);
            glUniform1iARB(m_shaderColorMap,0);
            glUniform1iARB(m_shaderNormalMap,1);
        }
    }

    if (!flag.isActive(DisplayFlag::WIREFRAME)) {
        glDrawElements(GL_TRIANGLES, triangles.size() * TTriangle::size(), GL_UNSIGNED_INT, &triangles[0]);
    } else {
        glDrawElements(GL_LINES, lines.size() * TLine::size(), GL_UNSIGNED_INT, &lines[0]);
    }

    if (tex) {
        if (texnormal) {
            glUseProgramObjectARB(0);
            glDisable(GL_TEXTURE_2D);
            glActiveTextureARB(GL_TEXTURE0_ARB);
            glDisableVertexAttribArrayARB(m_shaderTangentAttrib);
        }

        glDisableClientState(GL_NORMAL_ARRAY);

        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);
    }

    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
}


DECLARE_CLASS(OglModel)
DECLARE_ALIAS(OglShaderVisualModel,OglModel)
