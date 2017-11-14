#include <solver/CGLinearSolver.h>
#include <stdio.h>
#include <constraint/Constraint.h>
#include <core/Visitor.h>
#include <core/Context.h>
#include <forcefield/ForceField.h>

CGLinearSolver::CGLinearSolver()
: d_iteration("iterations",100,this)
, d_tolerance("tolerance",0.00001,this)
, d_threshold("threshold",0.00001,this) {}

void CGLinearSolver::buildMatrix(MechanicalParams param) {
    m_params = param;
}

//// TODO : implement this function
//void CGLinearSolver::mulMatrixVector(TVecId x, TVecId b) {}

//// TODO : implement this function
//void CGLinearSolver::solve(TVecId x, TVecId b) {}

DECLARE_CLASS(CGLinearSolver)
