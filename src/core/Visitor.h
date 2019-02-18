#ifndef VISITOR_H
#define VISITOR_H

#include <core/BaseObject.h>

class Visitor {
public :

    //Main constructor of Visitor, c indicates if subtrees will be processed by the visitor
    Visitor(bool c = false) {
        m_processChilds = c;
    }

    virtual bool processObject(BaseObject * o) = 0;

    void execute(Context * c) {
        c->processVisitor(*this);
    }

    Visitor & ref() {
        return *this;
    }

    bool m_processChilds;
};

#endif
