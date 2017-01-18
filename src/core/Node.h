#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>
#include <core/BaseObject.h>

class Node : public BaseObject , public Context {
public:

    Data<double> d_dt;
    Data<TVec3> d_gravity;

    Node();

    void removeObject(BaseObject * n);

    void init();

    void draw(DisplayFlag flag);

    void computeBBox(BoundingBox & bbox);

    void processVisitor(Visitor & v);

    BaseObject * get(std::string link);

    std::string getName();

    Context * getContext();

    Context * getParent();

    void attach(BaseObject * o);

    void detach(BaseObject * o);

    TVec3 getGravity();

    double getDt();

    State * getMstate();

    Topology * getTopology();

protected:

    std::vector<BaseObject *> m_childs;

    Context * getChild(std::string name);

    BaseObject * getObject(std::string name);

    //direct link to state and topology
    State * m_state;
    Topology * m_topology;

};

#endif
