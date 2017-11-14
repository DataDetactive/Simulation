#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <dataStructures.h>
#include <core/Data.h>
#include <core/Factory.h>
#include <core/Context.h>

#include <map>
#include <string>
#include <iostream>
#include <typeinfo>
#include <limits>

class BaseObject {

private :
    std::map<std::string,BaseData *> m_data;//must be declared first because it is used in the constructor of data

public:

    Data<std::string> d_name;
    Data<bool> d_printLog;

    BaseObject()
    : d_name("name",BaseData::getDefaultName(),this)
    , d_printLog("printLog",false,this) {}

    virtual void init() {}

    virtual void draw(DisplayFlag flag) {}

    virtual void computeBBox(BoundingBox & /*boox*/) {}

    virtual void setAttribute(const char * name,const char * value) {
        std::map<std::string,BaseData *>::iterator it = m_data.find(std::string(name));

        if (it == m_data.end()) {
            std::cerr << "Error the data " << name << " does not exist in " << getClassName() << std::endl;
        } else {
            it->second->setValue(value);
        }
    }

    std::string getClassName() {
        return typeid(*this).name();
    }

    std::string getName() {
        return d_name.getValue();
    }

    virtual Context * getContext() {
        return m_context;
    }

    void registerData(std::string & name, BaseData * d) {
        std::map<std::string,BaseData *>::iterator it = m_data.find(name);

        if (it == m_data.end()) m_data[name] = d;
        else std::cerr << "Error the data " << name << " is already registered" << std::endl;
    }

    void setContext(Context * c) {
        m_context = c;
    }

protected:
    Context * m_context;

};

#endif
