#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <Sofa/Vec.h>
#include <Sofa/Mat.h>
#include <string>
#include <vector>

typedef float TReal;
typedef sofa::defaulttype::Vec<3,TReal> TVec3;
typedef sofa::defaulttype::Vec<4,TReal> TVec4;
typedef sofa::defaulttype::Vec<6,TReal> TVec6;
typedef sofa::defaulttype::Vec<12,TReal> TVec12;
typedef sofa::defaulttype::Mat<3,3,TReal> TMat3x3;
typedef sofa::defaulttype::Mat<4,4,TReal> TMat4x4;
typedef sofa::defaulttype::Mat<6,6,TReal> TMat6x6;
typedef sofa::defaulttype::Mat<12,6,TReal> TMat12x6;
typedef sofa::defaulttype::Mat<6,12,TReal> TMat6x12;
typedef sofa::defaulttype::Mat<12,12,TReal> TMat12x12;

typedef sofa::helper::fixed_array<unsigned int, 2> TLine;
typedef sofa::helper::fixed_array<unsigned int, 3> TTriangle;
typedef sofa::helper::fixed_array<unsigned int, 4> TTetra;

typedef sofa::defaulttype::Vec<2,TReal> TTexCoord;
typedef sofa::defaulttype::Vec<4,TReal> TColor;

extern std::string getFullPath(const char * fn);

struct BoundingBox
{
    TVec3 min;
    TVec3 max;
};

typedef struct DisplayFlag_s {
    enum  DisplayMode {
        STATS,
        VISUAL,
        STATE,
        FORCEFIELD,
        MAPPING,
        WIREFRAME,
        Nflag
    };

    bool isActive(DisplayMode m) {
        return flag & (1<<m);
    }

    int flag;
} DisplayFlag;

typedef struct {
    enum VecID_d {
        restPosition,
        position,
        velocity,
        force,
        vec_x,
        vec_q,
        vec_d,
        vec_r,
        NVecID
    };

} VecID;

typedef VecID::VecID_d TVecId;

typedef struct MechanicalParams_s {
    double mfactor;
    double bfactor;
    double kfactor;

    MechanicalParams_s() {}

    MechanicalParams_s(double m, double d, double s) {
        mfactor = m;
        bfactor = d;
        kfactor = s;
    }

} MechanicalParams;

#endif
