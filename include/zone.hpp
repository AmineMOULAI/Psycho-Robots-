#ifndef ZONE_HPP
#define ZONE_HPP

#include <iostream>
#include <vector>
#include "common_types.hpp"


struct Task
{
    pos_t location;
    TaskStatus status;
    std::vector<int> CompletedBy;
} typedef Task_t;
 

struct ExplorationZone
{
    pos_t location;
    ZoneStatus status;
    std::vector<int> VisitedBy;
} typedef EZ_t;
 

#endif