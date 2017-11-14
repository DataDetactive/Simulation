#ifndef CGLINEARSOLVER_H
#define CGLINEARSOLVER_H

#include <solver/Solver.h>
#include <loader/Loader.h>
#include <state/State.h>
#include <forcefield/ForceField.h>
#include <forcefield/UniformMass.h>

class CGLinearSolver : public Solver {
public:

    Data<int> d_iteration;
    Data<double> d_tolerance;
    Data<double> d_threshold;

    CGLinearSolver();

    void buildMatrix(MechanicalParams params);

    void solve(TVecId x, TVecId b);

private :
    MechanicalParams m_params;

    int m_cg_iter;

    void mulMatrixVector(TVecId result, TVecId input);

};

#endif
