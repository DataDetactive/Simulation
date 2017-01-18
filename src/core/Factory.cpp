#include <core/Factory.h>
#include <vector>
#include <string>
#include <typeinfo>
#include <map>
#include <stdio.h>
#include <iostream>
#include <core/BaseObject.h>

static std::map<std::string,Creator > factory_map;

bool Factory::addFactory(const char * name,Creator c) {
    factory_map[std::string(name)] = c;

    return true;
}

BaseObject * Factory::getInstance(const std::string & classname) {
    std::map<std::string,Creator>::iterator it = factory_map.find(classname);

    if (it == factory_map.end()) return NULL;

    Creator c = it->second;
    BaseObject * obj = c();
    return obj;
}


