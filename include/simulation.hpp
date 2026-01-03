#ifndef SIMULATION_HPP
#define SIMULATION_HPP


#include <iostream>
#include <vector>
#include "robot.hpp"
#include "common_types.hpp"
#include "zone.hpp"

class Simulation
{
    private:
        int width, height;
        std::vector<Robot*> robots;
        std::vector<Task_t*> tasks;
        std::vector<EZ_t*> explorationZone;
        int step;
        std::vector<int> lastMoved;
    public:
        Simulation(int w, int h) : width(w), height(h), robots{}, tasks{}, explorationZone{}, step(0) {};
        ~Simulation() {};

    void init();
    void display_state();
    void run_step();

    const std::vector<Robot*>& get_robots() const {return robots;}
    const std::vector<Task_t*>& get_tasks() const {return tasks;}
    const std::vector<EZ_t*>& get_explorationZone() const {return explorationZone;}
};




#endif