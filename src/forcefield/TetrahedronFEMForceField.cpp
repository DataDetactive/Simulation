#include <forcefield/TetrahedronFEMForceField.h>
#include <state/State.h>
#include <stdio.h>
#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <core/Visitor.h>

TetrahedronFEMForceField::TetrahedronFEMForceField()
: d_youngModulus("youngModulus",100.0,this)
, d_poissonRatio("poissonRatio",0.4,this)
, d_method("method","large",this)
{}

//// TODO : implement this function
//void TetrahedronFEMForceField::init() {}

//// TODO : implement this function
//void TetrahedronFEMForceField::addForce(TVecId vf) {}

//// TODO : implement this function
//void TetrahedronFEMForceField::addDForce(MechanicalParams params,TVecId vdx,TVecId vdf) {}

DECLARE_CLASS(TetrahedronFEMForceField)

