#ifndef INTERACTIONS_UTILS
#define INTERACTIONS_UTILS

#include <math.h>
#include <vector>

#include "common_types.hpp"

double get_distance(pos_t a, pos_t b);

std::vector<NearbyRobot_t*> buildAndSortNRobots(
    const Robot& self,
    const std::vector<Robots*>& allRobots;
);




#endif