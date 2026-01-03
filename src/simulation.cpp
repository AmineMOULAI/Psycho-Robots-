#include <iostream>
#include <random>
#include <math.h>
#include <algorithm>
#include <vector>
#include <string>

#include "../include/simulation.hpp"
#include "../include/robot.hpp"
#include "../include/social.hpp"
#include "../include/worker.hpp"
#include "../include/explorer.hpp"
#include "../include/zone.hpp"
#include "../include/common_types.hpp"
#include "display_utils.hpp"






void Simulation::init() 
{
    std::cout << "Simulation initialized !" << "\n\n";

    /*std::random_device rd;
    std::mt19937 gen(rd());*/
    std::uniform_int_distribution<> dist_pos(0, G - 1);
    std::uniform_int_distribution<> dist_type(0, 2);

    int nbRobots;
    static const RobotType types[] = {
        RobotType::Social,
        RobotType::Worker,
        RobotType::Explorer
    };

    
    std::cout << "Enter number of Robots : " ;
    std::cin >> nbRobots;
    
    int nbE = 0, nbW = 0;
    // Create Robots
    for (int i = 0; i < nbRobots; i++)
    {
        pos_t p = {dist_pos(gen), dist_pos(gen)};
        const RobotType Rtype = types[dist_type(gen)];
        //std::cout << Rtype << "\n";
        switch (Rtype)
        {
            case RobotType::Social: {
                Social* s = new Social(p);
                this->robots.push_back(s);
                break;
            }
            case RobotType::Worker:{
                Worker* w = new Worker(p);
                this->robots.push_back(w);
                nbW++;
                break;
            }
            case RobotType::Explorer:{
                Explorer* e = new Explorer(p);
                this->robots.push_back(e);
                nbE++;
                break;

            }
            default:
                break;
        }
    }

    // Create Exploration Zones and Tasks

    int NbEZ = std::clamp(nbE * EZ_PER_EXPLORER, MIN_EZ, MAX_EZ); // Determine the nb of EZ based on number of Explorer robots
    int NbTasks = std::clamp(nbW * TASKS_PER_WORKER, MIN_TASKS, MAX_TASKS); // Determine the nb of Task based on number of Worker robots
    
    // Loading Tasks...
    for (int i = 0; i < NbTasks; i++)
    {
        pos_t posTask = {dist_pos(gen), dist_pos(gen)};
        Task_t* t = new Task_t{posTask, TaskStatus::Pending, {}};
        this->tasks.push_back(t);

    }

    // Loading Exploration Zones

    for (int i = 0; i < NbEZ; i++)
    {
        pos_t posEZ = {dist_pos(gen), dist_pos(gen)};
        EZ_t* ez = new EZ_t{posEZ, ZoneStatus::NotVisited, {}};
        this->explorationZone.push_back(ez);
    }
    
};

void Simulation::display_state() {
    // Restore original display sequence: title, grid, legend, zones, tasks, robots, stats
    DisplayUtils::displayTitle();
    DisplayUtils::displayGrid(G, G, robots, tasks, explorationZone);
    DisplayUtils::displayLegend();
    DisplayUtils::displayZonesInfo(explorationZone);
    DisplayUtils::displayTasksInfo(tasks);
    DisplayUtils::displayAllRobots(robots);
    DisplayUtils::displayStats(robots, explorationZone, tasks);
}

void Simulation::run_step()
{
    std::vector<Robot*> allRobots = this->get_robots();
    for (Robot* r : this->robots){
        r->update(allRobots);
        if (r->get_type() == RobotType::Explorer)
        {
            Explorer* e = dynamic_cast<Explorer*>(r);
            if (e) e->explore(this->explorationZone);
        }
    };
}