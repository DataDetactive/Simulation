#ifndef POSITIONCONSTRAINT_H
#define POSITIONCONSTRAINT_H

#include <core/BaseObject.h>
#include <constraint/Constraint.h>


class PositionConstraint : public Constraint {
public:

    Data<int> d_index;
    Data<TVec3> d_position;

    PositionConstraint();

    void applyConstraint(TVecId vf);

    void draw(DisplayFlag /*flag*/);


};
#endif
