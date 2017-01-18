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
//void EulerExplicit::step(double dt) {}

DECLARE_CLASS(EulerExplicit)
