#ifndef MYSIMPLETYPES_H
#define MYSIMPLETYPES_H

#include <stdint.h>

namespace MyTypes
{

struct SPointType
{
    int16_t x {};
    int16_t y {};
    SPointType(int16_t x = 0, int16_t y = 0):
            x(x),
            y(y)
    {}
};


struct SMarkerType
{
    float x {};
    float y {};
    SMarkerType(float x = 0, float y = 0) :
            x(x),
            y(y)
    {}
};

const float MARKERS_SCALE = 100.0f;
const float POINTS_SCALE = 1.0f/18.4f;
//const int MAX_POINTS_FIELD_SIZE = 1200;
const int MARKERS_LENGTH = 10;


}

#endif // MYSIMPLETYPES_H
