#ifndef OGLTEXTURE_H
#define OGLTEXTURE_H

#include <visual/Visual.h>
#include <core/BaseObject.h>
#include <dataStructures.h>
#include <vector>
#include <topology/Topology.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

typedef enum {
    TEXTURE_COLOR,
    TEXTURE_NORMAL,
    TEXTURE_UNDEF
} TextureType;

class OglTexture : public BaseObject {
public:

    Data<std::string> d_textureFilename;
    Data<std::string> d_textureId;
    Data<int> d_textureUnit;//for sofa compatibility
    Data<bool> d_repeat;//for sofa compatibility

    OglTexture();

    TextureType getType();

    GLint getTextureId();

    void init();

    GLint loadTexture(const std::string& filename);

protected:

    GLint m_textureId;
    TextureType m_type;

};

class FindTextureIdVisitor : public Visitor {
public:
    FindTextureIdVisitor(TextureType t) {
        m_tid = -1;
        m_type = t;
    }

    bool processObject(BaseObject * o)  {
        if (OglTexture * t = dynamic_cast<OglTexture *>(o)) {
            if (t->getType() == m_type) {
                m_tid = t->getTextureId();
                return false;
            }
        }

        return true;
    }

    GLint find(Context * c) {
        c->processVisitor(*this);
        return m_tid;
    }

public:
    TextureType m_type;
    GLint m_tid;
};

#endif
