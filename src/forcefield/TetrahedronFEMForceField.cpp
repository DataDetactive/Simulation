#include <forcefield/TetrahedronFEMForceField_optim.h>
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



