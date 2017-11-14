#ifndef CONTEXT_H
#define CONTEXT_H

#include <iostream>
#include <sstream>
#include <string>
#include <dataStructures.h>

class Visitor;
class State;
class Topology;

class Context {
public :

    //return the Context as BaseObject
    virtual BaseObject * get(std::string link)  = 0;

    //Launch a visitor from this context
    virtual void processVisitor(Visitor & v) = 0;

    //Get Parent of the Context
    virtual Context * getParent()  = 0;

    //Return a pointer on the context of the child object with name name
    virtual Context * getChild(std::string name) = 0;

    virtual BaseObject * getObject(std::string name) = 0;

    virtual std::string getName() = 0;

    //return the gravity of the scene
    virtual TVec3 getGravity() = 0;

    //return the time step of the scene
    virtual double getDt() = 0;

    //return a pointer to the first State of the context (direct access)
    virtual State * getMstate() = 0;

    //return a pointer to the topology of the context (direct access)
    virtual Topology * getTopology() = 0;

};

#endif
