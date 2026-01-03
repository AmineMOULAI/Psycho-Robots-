#include <math.h>
#include <vector>
#include <algorithm>

#include "../include/common_types.hpp"
#include "../include/explorer.hpp"
#include "../include/interactions_utils.hpp"
#include "../include/robot.hpp"
#include "../include/simulation.hpp"
#include "../include/social.hpp"
#include "../include/worker.hpp"
#include "../include/zone.hpp"


double get_distance(pos_t a, pos_t b) {return std::sqrt(std::pow((a.x - b.x), 2) + std::pow((a.y - b.y), 2));};

NearbyRobot_t buildAndSortNRobots(
    const Robot& self,
    const std::vector<Robot*>& allRobots )
{
    std::vector<NearbyRobot_t> candidates;

    for (Robot* other : allRobots)
    {
        if (&self != other)
        {
            double d = get_distance(self.get_pos(), other->get_pos());

            if (d <= RADIUS)
            {
                NearbyRobot_t candid;
                candid.Nid = other->get_id();
                candid.distance = d;
                candid.SameType = (self.get_type() == other->get_type());

                candidates.push_back(candid);
            }
        }
    }

    if(candidates.empty()) return NearbyRobot_t{-1, 0.0, false};
    if (candidates.size() == 1) return candidates[0];

    sort(candidates.begin(), candidates.end(), 
        [](const NearbyRobot_t& r1, const NearbyRobot_t& r2) {

            if (r1.distance != r2.distance) return r1.distance < r2.distance;
            if (r1.SameType != r2.SameType) return r1.SameType > r2.SameType;
            return r1.Nid < r2.Nid;
            
        }

    );  

    return candidates[0];
    
}

