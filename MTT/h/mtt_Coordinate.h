#pragma once

#include "mtt_utility.h"

#include <utility>
#include <vector>

#include <QString>
#include <QVariant>

class Coordinate
{
public:
    Coordinate() = default;
    ~Coordinate() = default;

   utilities::Geo_Coordinate geo_coord;
   utilities::Map_Coordinate map_coord;
   QString area = "";
   QVariant value;
};
