#include <solver/CGLinearSolver.h>
#include <stdio.h>
#include <constraint/Constraint.h>
#include <core/Visitor.h>
#include <core/Context.h>
#include <forcefield/ForceField.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

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

void CGLinearSolver::draw(DisplayFlag flag) {
    if (!flag.isActive(DisplayFlag::STATS)) return;

    char buf[256];
    sprintf(buf,"CG iterations: %d ",m_cg_iter);

    glDisable( GL_DEPTH_TEST ) ; // also disable the depth test so renders on top

    int i=0;
    while (buf[i] != 0) {
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, buf[i] );
        i++;
    }

    glEnable( GL_DEPTH_TEST ) ; // Turn depth testing back on
}


DECLARE_CLASS(CGLinearSolver)
