#ifndef VISUAL_H
#define VISUAL_H

#include <loader/Loader.h>
#include <integrator/Integrator.h>
#include <GL/glew.h>
#include <GL/gl.h>

template<class T>
GLenum myglType();

class VisualSofa : public BaseObject {
public:
    Data<std::string> d_id;
    Data<std::string> d_value;

    VisualSofa();
};

#endif
