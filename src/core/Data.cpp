#include <core/Data.h>
#include <core/BaseObject.h>
#include <string>
#include <sstream>

static int s_nameId=0;

std::string BaseData::getDefaultName() {
    std::stringstream ss;
    ss << "default" << s_nameId++;
    return ss.str();
}

template<class T>
Data<T>::Data(std::string name, T v,BaseObject * o) {
    m_value = v;
    o->registerData(name,this);
}

template<>
void Data<std::string>::setValue(const char * v) {
    m_value = std::string(v);
}

template<>
void Data<int>::setValue(const char * v) {
    m_value = atoi(v);
}

template<>
void Data<bool>::setValue(const char * v) {
    std::string s(v);
    if (s == "true") m_value = true;
    else if (s == "false") m_value = false;
    else m_value = atoi(v);
}

template<>
void Data<float>::setValue(const char * v) {
    float res;

    std::string line(v);
    std::stringstream ss(line);
    ss >> res;

    m_value = res;
}

template<>
void Data<double>::setValue(const char * v) {
    float res;

    std::string line(v);
    std::stringstream ss(line);
    ss >> res;

    m_value = res;
}

template<>
void Data<TVec3>::setValue(const char * v) {
    TVec3 res;

    std::string line(v);
    std::stringstream ss(line);
    ss >> res[0] >> res[1] >> res[2];

    m_value = res;
}

template<>
void Data<TVec6>::setValue(const char * v) {
    TVec6 res;

    std::string line(v);
    std::stringstream ss(line);
    ss >> res[0] >> res[1] >> res[2] >> res[3] >> res[4] >> res[5];

    m_value = res;
}

template<>
void Data<BoundingBox>::setValue(const char * v) {
    BoundingBox res;

    std::string line(v);
    std::stringstream ss(line);
    ss >> res.min[0] >> res.min[1] >> res.min[2] >> res.max[0] >> res.max[1] >> res.max[2];

    m_value = res;
}

template class Data<int>;
template class Data<bool>;
template class Data<float>;
template class Data<double>;
template class Data<std::string>;
template class Data<TVec3>;
template class Data<TVec6>;
template class Data<BoundingBox>;
