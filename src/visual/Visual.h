#ifndef VISUAL_H
#define VISUAL_H

#include <loader/Loader.h>
#include <integrator/Integrator.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

template<class T>
GLenum myglType();

class VisualSofa : public BaseObject {
public:
    Data<std::string> d_id;
    Data<std::string> d_value;
    Data<std::string> d_shaderVertex;
    Data<std::string> d_shaderFragment;

    VisualSofa();
};

#endif
