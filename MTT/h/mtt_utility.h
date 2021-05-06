#ifndef UTILITY_H
#define UTILITY_H

#include <utility>

namespace utilities
{

struct Geo_Coordinate
{
    double latitude = 0.; //y
    double longitude = 0.; //x
};

struct Map_Coordinate
{
    double x = 0.;
    double y = 0.;
};

Map_Coordinate CalculateMapPosition(const Geo_Coordinate& x, const std::pair<Geo_Coordinate, Map_Coordinate>& ref1, const std::pair<Geo_Coordinate, Map_Coordinate>& ref2);

}

#endif
