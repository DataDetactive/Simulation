#ifndef FACTORY_H
#define FACTORY_H

#include <vector>
#include <string>
#include <typeinfo>
#include <map>
#include <stdio.h>
#include <iostream>

class BaseObject;

class Context;

typedef BaseObject * (*Creator)();

class Factory {
public:
    static BaseObject * getInstance(const std::string & classname);

    static bool addFactory(const char * name,Creator c);
};

#define DECLARE_CLASS(CLASS_NAME) \
static BaseObject * createInstance##CLASS_NAME() { \
    return new CLASS_NAME(); \
} \
bool declare_init##CLASS_NAME = Factory::addFactory(#CLASS_NAME,createInstance##CLASS_NAME);

#define DECLARE_ALIAS(ALIAS,CLASS_NAME) \
static BaseObject * createInstance##ALIAS() { \
    return new CLASS_NAME(); \
} \
bool declare_init##ALIAS = Factory::addFactory(#ALIAS,createInstance##ALIAS);

#endif
