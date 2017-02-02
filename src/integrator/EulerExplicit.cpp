#include <stdio.h>
#include <integrator/EulerExplicit.h>
#include <constraint/Constraint.h>
#include <core/Visitor.h>
#include <core/Context.h>
#include <forcefield/ForceField.h>
#include <dataStructures.h>

EulerExplicit::EulerExplicit()
: d_vdamping("vdamping",1.0,this) {}

////// TODO : implement this function
//void EulerExplicit::step(double dt) {
    //f = 0

    // add internal forces f += forces

    // add mapped forces on the mechanical state f += mappedForce

    // solve M a = f

    // apply constraints i.e. f[constraint] = 0

    // Apply solution:  v = v + h a

    // v = v*d_vdamping

    // x = x + h v
//}

DECLARE_CLASS(EulerExplicit)
