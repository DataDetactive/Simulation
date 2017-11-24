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
#include <visual/OglShader.h>

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
        TVec3 n = B.cross(C);
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
        m_tangents[i] = m_normals[i].cross(m_tangents[i]);
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


    GLint m_shaderVertex = -1;
    GLint m_shaderFragment = -1;
    GLint m_shaderProgram = -1;
    GLint m_shaderTangentAttrib = -1;
    GLint m_shaderColorMap = -1;
    GLint m_shaderNormalMap = -1;

    OglShader * shader = FindVisitor<OglShader>::find(this->getContext());

    if (shader != NULL) {
        m_shaderVertex = shader->getShaderVertex();
        m_shaderFragment = shader->getShaderFragment();
        m_shaderProgram = shader->getShaderProgram();
        m_shaderTangentAttrib = shader->getShaderTangentAttrib();
        m_shaderColorMap = shader->getShaderColorMap();
        m_shaderNormalMap = shader->getShaderNormalMap();
    }

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
