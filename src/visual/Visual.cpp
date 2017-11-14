#include <visual/Visual.h>

VisualSofa::VisualSofa()
: d_id("id","",this)
, d_value("value","",this) {

}

template<>
GLenum myglType<float>() {
    return GL_FLOAT;
}

template<>
GLenum myglType<double>() {
    return GL_DOUBLE;
}


DECLARE_ALIAS(OglFloatVariable,VisualSofa)
DECLARE_ALIAS(OglFloat2Variable,VisualSofa)
DECLARE_ALIAS(OglShaderDefineMacro,VisualSofa)
