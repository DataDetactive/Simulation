#include <stdio.h>
#include <integrator/EulerImplicit.h>
#include <constraint/Constraint.h>
#include <core/Visitor.h>
#include <core/Context.h>
#include <solver/Solver.h>
#include <mapping/Mapping.h>

EulerImplicit::EulerImplicit()
: d_rayleighMass("rayleighMass",0.1,this)
, d_rayleighStiffness("rayleighStiffness",0.1,this)
, d_vdamping("vdamping",1.0,this)
{}

////// TODO : implement this function
//void EulerImplicit::step(double dt) {}

DECLARE_CLASS(EulerImplicit)
