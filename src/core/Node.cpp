#include <core/Factory.h>
#include <core/Node.h>
#include <stdio.h>
#include <core/Visitor.h>
#include <core/BaseObject.h>
#include <state/State.h>
#include <topology/Topology.h>

Node::Node()
: d_dt("dt",0.001,this)
, d_gravity("gravity",TVec3(0,-1,0),this)
{
    this->m_context = this;

    m_state = NULL;

    m_topology = NULL;
}

void Node::init() {


    for (unsigned i=0;i<m_childs.size();i++) {
        m_childs[i]->init();
    }

    m_bbox.min = TVec3(std::numeric_limits<TReal>::max(),std::numeric_limits<TReal>::max(),std::numeric_limits<TReal>::max());
    m_bbox.max = TVec3(std::numeric_limits<TReal>::min(),std::numeric_limits<TReal>::min(),std::numeric_limits<TReal>::min());

    computeBBox(m_bbox);
}

void Node::computeBBox(BoundingBox & bbox) {
    m_bbox.min = bbox.min;
    m_bbox.max = bbox.max;

    for (unsigned i=0;i<m_childs.size();i++) {
        m_childs[i]->computeBBox(m_bbox);
    }

    if (m_bbox.min[0] < bbox.min[0]) bbox.min[0] = m_bbox.min[0];
    if (m_bbox.min[1] < bbox.min[1]) bbox.min[1] = m_bbox.min[1];
    if (m_bbox.min[2] < bbox.min[2]) bbox.min[2] = m_bbox.min[2];

    if (m_bbox.max[0] > bbox.max[0]) bbox.max[0] = m_bbox.max[0];
    if (m_bbox.max[1] > bbox.max[1]) bbox.max[1] = m_bbox.max[1];
    if (m_bbox.max[2] > bbox.max[2]) bbox.max[2] = m_bbox.max[2];
}

State * Node::getMstate() {
    return m_state;
}

Topology * Node::getTopology() {
    return m_topology;
}

void Node::draw(DisplayFlag flag) {
    for (unsigned i=0;i<m_childs.size();i++) {
        m_childs[i]->draw(flag);
    }
}

void Node::processVisitor(Visitor & v) {
    for (unsigned i=0;i<m_childs.size();i++) {
        if (dynamic_cast<Node * >(m_childs[i])) {
            Node * n = (Node *) m_childs[i];

            if (v.m_processChilds) n->processVisitor(v);
        }

        if (! v.processObject(m_childs[i])) return;
    }
}

Context * Node::getChild(std::string name) {
    if (name=="..") return this->getParent();
    if (name==".") return this;

    for (unsigned i=0;i<m_childs.size();i++) {
        if (m_childs[i]->getName() == name) return m_childs[i]->getContext();
    }

    return NULL;
}

Context * Node::getContext() {
    return this;
}

Context *  Node::getParent() {
    return this->m_context;
}

void Node::attach(BaseObject * o) {
    m_childs.push_back(o);
    o->setContext(this);

    if ((m_state == NULL) && (dynamic_cast<State*>(o))) m_state = (State*)o;
    if ((m_topology == NULL) && (dynamic_cast<Topology*>(o))) m_topology = (Topology*)o;
}

void Node::detach(BaseObject * o) {
    for (unsigned i=0;i<m_childs.size();i++) {
        if (m_childs[i] == o) {
            m_childs.erase(m_childs.begin()+i);

            if (o == (BaseObject*) m_state) m_state = NULL;
            if (o == (BaseObject*) m_topology) m_topology = NULL;

            o->setContext(NULL);
            return;
        }
    }
}

BaseObject * Node::getObject(std::string name) {
    if (name=="..") return this->getParent()->getObject(".");
    if (name==".") return this;

    for (unsigned i=0;i<m_childs.size();i++) {
        if (m_childs[i]->getName() == name) return m_childs[i];
    }

    return NULL;
}

TVec3 Node::getGravity() {
    return d_gravity.getValue();
}

double Node::getDt() {
    return d_dt.getValue();
}

BaseObject * Node::get(std::string link) {
    Context * context = this;
    std::string name;

    std::string::size_type pos = link.find_last_of("/\\");

    if (pos == std::string::npos) {
        name = link;
    } else {
        name = link.substr(pos+1,link.length());

        std::string path = link.substr(0,pos);
        std::stringstream ss;
        ss.str(path);
        std::string item;

        while (std::getline(ss, item, '/')) {
            Context * c = context->getChild(item);
            if (c == NULL) break;
            context = c;
        }

    }

    return context->getObject(name);
}

std::string Node::getName() {
    return BaseObject::getName();
}

DECLARE_CLASS(Node)


