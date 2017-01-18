#include <loader/Loader.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <core/Visitor.h>

Loader::Loader()
: BaseObject()
, d_filename("filename",std::string(),this)
, d_scale3d("scale3d",TVec3(1,1,1),this)
{

}

void Loader::init() {
    Topology * topo = FindVisitor<Topology>().find(this->getContext());

    if (topo == NULL) {
        std::cerr << "Error the loader " << this->getName() << " cannot find the topology" << std::endl;
        return;
    }


    FindVisitor<State> v2;
    this->getContext()->processVisitor(v2);
    State * state = v2.getObject();

    if (state == NULL) {
        std::cerr << "Error the loader " << this->getName() << " cannot find the state" << std::endl;
        return;
    }


    load(topo,state, getFullPath(d_filename.getValue().c_str()).c_str());
}


