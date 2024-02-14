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
void TetrahedronFEMForceField::init() {
    // Converting E and v To lambda and mu
        TReal E = d_youngModulus.getValue();
        TReal v = d_poissonRatio.getValue();

        TReal lambda = (E*v)/((1+v)*(1-2*v));
        TReal mu = E/(2*(1+v));

        // Filling the De matrix
        TMat6x6 De;
        De = TMat6x6(TMat6x6::Line(lambda+2*mu,lambda,lambda,0,0,0),
                     TMat6x6::Line(lambda,lambda+2*mu,lambda,0,0,0),
                     TMat6x6::Line(lambda,lambda,lambda+2*mu,0,0,0),
                     TMat6x6::Line(0,0,0,mu,0,0),
                     TMat6x6::Line(0,0,0,0,mu,0),
                     TMat6x6::Line(0,0,0,0,0,mu)
                    );

        // Computing for each Tetrahedrom
        TMat4x4 Ve;
        TMat4x4 Ve_inv;
        TReal b1,b2,b3,b4,c1,c2,c3,c4,d1,d2,d3,d4;

        TMat6x12 Be;
        TReal det_Ve;
        TMat3x3 m1,m2,m3,m4;
        TMat3x3::Line l1,l2,l3,l4;

        const std::vector<TTetra> & tetras = getContext()-> getTopology() -> getTetras();
        const std::vector<TVec3> & position = getContext() -> getMstate() -> get(VecID::position);
        TMat12x12 Ke;

        int n_thetra = tetras.size();

        for(int i=0;i<n_thetra;i++){

            // Matrix Ve
            for(int j=0;j<4;j++){
                Ve[j][0] = 1;
                Ve[j][1] = position[tetras[i][j]][0]; //xi
                Ve[j][2] = position[tetras[i][j]][1]; //yi
                Ve[j][3] = position[tetras[i][j]][2]; //zi
            }

            Ve_inv = Ve.inverse();

            b1 = Ve_inv[1][0];
            b2 = Ve_inv[1][1];
            b3 = Ve_inv[1][2];
            b4 = Ve_inv[1][3];
            c1 = Ve_inv[2][0];
            c2 = Ve_inv[2][1];
            c3 = Ve_inv[2][2];
            c4 = Ve_inv[2][3];
            d1 = Ve_inv[3][0];
            d2 = Ve_inv[3][1];
            d3 = Ve_inv[3][2];
            d4 = Ve_inv[3][3];

            Be = TMat6x12(TMat6x12::Line(b1, 0,0,b2,0,0,b3,0,0,b4,0,0),
                          TMat6x12::Line(0,c1,0,0,c2,0,0,c3,0,0,c4,0),
                          TMat6x12::Line(0,0,d1,0,0,d2,0,0,d3,0,0,d4),
                          TMat6x12::Line(c1,b1,0,c2,b2,0,c3,b3,0,c4,b4,0),
                          TMat6x12::Line(d1,0,b1,d2,0,b2,d3,0,b3,d4,0,b4),
                          TMat6x12::Line(0,d1,c1,0,d2,c2,0,d3,c3,0,d4,c4)
                          );

            l1 = TMat3x3::Line(Ve[0][1],Ve[0][2],Ve[0][3]);
            l2 = TMat3x3::Line(Ve[1][1],Ve[1][2],Ve[1][3]);
            l3 = TMat3x3::Line(Ve[2][1],Ve[2][2],Ve[2][3]);
            l4 = TMat3x3::Line(Ve[3][1],Ve[3][2],Ve[3][3]);

            m1 = TMat3x3(l2,l3,l4);
            m2 = TMat3x3(l1,l3,l4);
            m3 = TMat3x3(l1,l2,l4);
            m4 = TMat3x3(l1,l2,l3);

            det_Ve = determinant(m1) - determinant(m2) + determinant(m3) - determinant(m4);

            Ke = (det_Ve/6)*(Be.transpose())*De*Be;
            m_vecKe.push_back(Ke);

        }
}

//// TODO : implement this function
//void TetrahedronFEMForceField::addForce(TVecId vf) {}

//// TODO : implement this function
//void TetrahedronFEMForceField::addDForce(MechanicalParams params,TVecId vdx,TVecId vdf) {}

DECLARE_CLASS(TetrahedronFEMForceField)

