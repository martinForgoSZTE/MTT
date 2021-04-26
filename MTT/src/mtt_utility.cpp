#include "mtt_utility.h"

namespace utilities
{

static double CalculateY_PartOfPosition(const double& x, const double& x1, const double& y1, const double& x2, const double& y2)
{
    return (y1 + ((x - x1)*(y2 - y1)) / (x2 - x1));
}

static Map_Coordinate CalculateMapPosition(const Geo_Coordinate& x, const std::pair<Geo_Coordinate, Map_Coordinate>& ref1, const std::pair<Geo_Coordinate, Map_Coordinate>& ref2)
{
    Map_Coordinate y;
    y.width = CalculateY_PartOfPosition(x.latitude, ref1.first.latitude, ref1.second.width, ref2.first.latitude, ref2.second.width);
    y.height = CalculateY_PartOfPosition(x.longitude, ref1.first.longitude, ref1.second.height, ref2.first.longitude, ref2.second.height);

    return y;
}


}
