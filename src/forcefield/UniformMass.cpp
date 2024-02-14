#include <stdio.h>
#include <forcefield/UniformMass.h>
#include <state/State.h>

UniformMass::UniformMass()
: d_mass("mass",1.0,this)
{}

//// TODO : implement this function
void UniformMass::addForce(TVecId v_f) {
    State * state = this->getContext()->getMstate();

        auto grav = this->getContext()->getGravity();

        std::vector<TVec3> & X = state-> get(v_f);


        for(unsigned pp=0;pp<state->size();pp++){
       X[pp] =  grav*d_mass.getValue() ;
        }
}

//// TODO : implement this function
//void UniformMass::addDForce(MechanicalParams params,TVecId vdx,TVecId vdf) {}

//// TODO : implement this function
//void UniformMass::accFromF(TVecId va, TVecId vf) {}

DECLARE_CLASS(UniformMass)
