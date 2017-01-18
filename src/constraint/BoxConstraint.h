#ifndef BOXCONSTRAINT_H
#define BOXCONSTRAINT_H

#include <core/BaseObject.h>
#include <constraint/Constraint.h>

class BoxConstraint : public Constraint {
public:

    Data<BoundingBox> d_box;

    BoxConstraint();

    void init();

    void draw(DisplayFlag flag);

    void applyConstraint(TVecId f);

protected:
    std::vector<int> m_indices;

};
#endif
