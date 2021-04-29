#include "mtt_utility.h"

namespace utilities
{

static double CalculateY_PartOfPosition(const double& x, const double& x1, const double& y1, const double& x2, const double& y2)
{
    return (y1 + ((x - x1)*(y2 - y1)) / (x2 - x1));
}

Map_Coordinate CalculateMapPosition(const Geo_Coordinate& coordXi, const std::pair<Geo_Coordinate, Map_Coordinate>& ref1, const std::pair<Geo_Coordinate, Map_Coordinate>& ref2)
{
    Map_Coordinate coordYi;
    coordYi.y = CalculateY_PartOfPosition(coordXi.latitude, ref1.first.latitude, ref1.second.y, ref2.first.latitude, ref2.second.y);
    coordYi.x = CalculateY_PartOfPosition(coordXi.longitude, ref1.first.longitude, ref1.second.x, ref2.first.longitude, ref2.second.x);

    return coordYi;
}


}
