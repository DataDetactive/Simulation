#include <animationloop/DefaultAnimationLoop.h>
#include <stdio.h>
#include <loader/Loader.h>
#include <core/Visitor.h>
#include <integrator/Integrator.h>
#include <mapping/Mapping.h>


//// TODO : implement this function
void DefaultAnimationLoop::step(double time) {
    // create a member of the class that we have just created in the animation.h file
    ApplyIntegratorVisitor a(this->getContext()->getDt());
    // carry out the process
    this->getContext()->processVisitor(a);
     ApplyMappingVisitor b;
     this->getContext()->processVisitor(b);
}

DECLARE_CLASS(DefaultAnimationLoop)

