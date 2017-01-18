#include <stdio.h>
#include <constraint/BoxConstraint.h>
#include <topology/Topology.h>
#include <state/State.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <core/Visitor.h>
#include <core/Context.h>

BoxConstraint::BoxConstraint()
: d_box("box",BoundingBox(),this) {}

//// TODO : implement this function
//void BoxConstraint::init() {}

//// TODO : implement this function
//void BoxConstraint::applyConstraint(TVecId vf) {}

//// TODO : implement this function
//void BoxConstraint::draw(DisplayFlag flag) {}

DECLARE_CLASS(BoxConstraint)
