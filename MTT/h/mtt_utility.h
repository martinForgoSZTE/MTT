#ifndef UTILITY_H
#define UTILITY_H

#include <utility>

namespace utilities
{

struct Geo_Coordinate
{
    double latitude; //x
    double longitude; //y
};

struct Map_Coordinate
{
    double width;
    double height;
};

static Map_Coordinate CalculateMapPosition(const Geo_Coordinate& x, const std::pair<Geo_Coordinate, Map_Coordinate>& ref1, const std::pair<Geo_Coordinate, Map_Coordinate>& ref2);

}

#endif
