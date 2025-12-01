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

void Simulation::display_state() 
{
    std::vector<std::string> grid(G, std::string(G, '.'));
    
    // Loading Robots...
    for (std::size_t i = 0; i < this->robots.size(); i++)
    {
        switch (this->robots[i]->get_type())
        {
            case RobotType::Social:
                grid[this->robots[i]->get_pos().x][this->robots[i]->get_pos().y] = 'S';
                break;
            case RobotType::Worker:
                grid[this->robots[i]->get_pos().x][this->robots[i]->get_pos().y] = 'W';
                break;
            case RobotType::Explorer:
                grid[this->robots[i]->get_pos().x][this->robots[i]->get_pos().y] = 'E';
                break;
            default:
                break;
        }
    }

    // Loading Tasks...
    for (std::size_t i = 0; i < this->tasks.size(); i++)
    {
        int x = this->tasks[i]->location.x;
        int y = this->tasks[i]->location.y;
        grid[x][y] = 'T';    

    }
    // Loading Exploration Zones
    for (std::size_t i = 0; i < this->explorationZone.size(); i++)
    {
        int x = this->explorationZone[i]->location.x;
        int y = this->explorationZone[i]->location.y;
        grid[x][y] = 'Z';
    }
    
    
    for (int i = 0; i < G; i++)
        std::cout << "##";
    std::cout << "###\n";
    for (std::size_t i = 0; i < grid.size(); i++)
    {
        std::cout << "#";
        for (std::size_t j = 0; j < grid[i].size(); j++)
            std::cout << ' ' << grid[i][j];
        std::cout << " #\n";
    }
    for (int i = 0; i < G; i++)
        std::cout << "##";
    std::cout << "###\n";

    for (Robot* r : this->robots)
    {
        r->display();
        std::cout << "\n";
    }
    
};

void Simulation::run_step()
{
    for (Robot* r : this->robots){r->update();}; 
}