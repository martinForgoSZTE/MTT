#pragma once

#include "mtt_utility.h"

#include <utility>
#include <vector>

#include <QString>
#include <QVariant>
#include <QBrush>

class Coordinate
{
public:
    Coordinate() = default;
    ~Coordinate() = default;

    
   utilities::Geo_Coordinate geo_coord;
   utilities::Map_Coordinate map_coord;
   QString area = "";
   QBrush color;
   bool operator==(const Coordinate& rhs)
   {
       return area == rhs.area &&
               geo_coord.latitude == rhs.geo_coord.latitude &&
               geo_coord.longitude == rhs.geo_coord.longitude &&
               map_coord.x == rhs.map_coord.x &&
               map_coord.y == rhs.map_coord.y &&
               color == rhs.color;
   }
};
