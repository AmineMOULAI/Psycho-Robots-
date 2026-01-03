#include <iostream>
#include <algorithm>
#include <cmath>
#include "../include/explorer.hpp"
#include "../include/common_types.hpp"

void Explorer::explore(std::vector<EZ_t*>& explorationZones)
{
    // Check for unexplored zones nearby
    for (auto zone : explorationZones)
    {
        double distance = std::sqrt(
            std::pow(pos.x - zone->location.x, 2) + 
            std::pow(pos.y - zone->location.y, 2)
        );
        
        // If close to a zone, visit it if not already visited
        if (distance <= EXPLORER_ZONE_VISIT_DISTANCE && zone->status == ZoneStatus::NotVisited)
        {
            zone->status = ZoneStatus::Visited;
            zone->VisitedBy.push_back(r_id);
            exploredZones.push_back(zone->location);
            
            // Add to known zones to transmit in interactions
            if (std::find(knownZones.begin(), knownZones.end(), zone) == knownZones.end())
            {
                knownZones.push_back(zone);
            }
            
            std::cout << "Explorer " << r_id << " visited zone at (" 
                      << zone->location.x << ", " << zone->location.y << ")" << std::endl;
        }
        // Add nearby zones to known zones even if not visited
        else if (distance <= RADIUS)
        {
            if (std::find(knownZones.begin(), knownZones.end(), zone) == knownZones.end())
            {
                knownZones.push_back(zone);
            }
        }
    }
    
    // Check if all zones are explored
    bool allVisited = true;
    for (auto zone : explorationZones)
    {
        if (zone->status == ZoneStatus::NotVisited)
        {
            allVisited = false;
            break;
        }
    }
    
    // If all zones are explored, request new environment
    if (allVisited && exploredZones.size() == explorationZones.size())
    {
        wantsNewEnvironment = true;
        std::cout << "Explorer " << r_id << " has explored all zones and wants a new environment!" << std::endl;
        
        // Decrease curiosity as it's explored everything
        set_curiosity(get_curiosity() - EXPLORER_CURIOSITY_LOSS);
    }
}

void Explorer::update(std::vector<Robot*>& allRobots)
{
    Robot::update(allRobots);
    
    // Explorers leave if they want new environment (all zones explored) or energy is too low
    if ((wantsNewEnvironment && get_curiosity() < 30) || get_energy() <= 0)
    {
        hasLeft = true;
        std::cout << "Explorer " << r_id << " has left the simulation.\n";
    }
}

void Explorer::interact(Robot& r)
{
    std::cout << "Explorer " << r.get_id() << " transmits zone information to ";
    
    switch (r.get_type())
    {
    case RobotType::Social:
        std::cout << "Social";
        break;
    case RobotType::Worker:
        std::cout << "Worker";
        break;
    case RobotType::Explorer:
        std::cout << "Explorer";
        break;
    default:
        break;
    }
    
    std::cout << r.get_id();
    
    if (!knownZones.empty())
    {
        std::cout << " - Known zones: " << knownZones.size();
    }
    
    std::cout << " !\n";
    
    // Apply interaction state changes
    set_stress(get_stress() - 5);
    set_energy(get_energy() + 3);
    set_curiosity(get_curiosity() + 3);
    
    r.set_stress(r.get_stress() - 5);
    r.set_energy(r.get_energy() + 3);
    r.set_curiosity(r.get_curiosity() + 3);
}

void Explorer::move()
{
    Robot::move();
}

void Explorer::display() const
{
    Robot::display();
    std::cout << "Explored Zones: " << this->get_exploredZones() << "\n";
    std::cout << "Wants New Environment: " << (wantsNewEnvironment ? "Yes" : "No") << "\n\n";
}

bool Explorer::respondTo(Robot& caller)
{
    return true;
}
