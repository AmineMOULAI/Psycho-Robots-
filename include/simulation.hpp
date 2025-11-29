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
        
    public:
        Simulation(int w, int h) : width(w), height(h), robots{}, tasks{}, explorationZone{} {};
        ~Simulation() {};

    void init();
    void display_state();
};




#endif