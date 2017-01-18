#ifndef DATA_H
#define DATA_H

#include <string>
#include <stdio.h>
#include <stdlib.h>

class BaseObject;

class BaseData {
public:
    virtual void setValue( const char *) = 0;

    static std::string getDefaultName();
};


template<class T>
class Data : public BaseData {
public :

    Data(std::string name, T v,BaseObject * o);

    T getValue() {
        return m_value;
    }

    void setValue(T v) {
        m_value = v;
    }

    void setValue(const char * v);

private :
    T m_value;

};

#endif
