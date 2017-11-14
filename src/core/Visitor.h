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

template<class T>
class FindVisitor : public Visitor {
public:

    FindVisitor() {
        m_object = NULL;
    }

    bool processObject(BaseObject * o)  {
        if (dynamic_cast<T *>(o)) {
            m_object = (T *) o;
            return false;
        }

        return true;
    }

    T * getObject() {
        return m_object;
    }

    static T * find(Context * c) {
        FindVisitor<T> visitor;
        visitor.execute(c);
        return visitor.getObject();
    }

public:
    T * m_object;
};

#endif
