#ifndef EXPLORER_HPP
#define EXPLORER_HPP

#include <iostream>
#include <vector>
#include "robot.hpp"
#include "common_types.hpp"

class Explorer : public Robot
{
    private:
        std::vector<pos_t> exploredZones;
    public:
        Explorer(pos_t p) : Robot(RobotType::Explorer, p, EXPLORER_STRESS, EXPLORER_ENERGY, EXPLORER_CURIOSITY), exploredZones{} {};
        ~Explorer(){};

    int get_exploredZones() const {return exploredZones.size();};

    // Call/Response Protocol
    bool respondTo(Robot& caller) override;


    void update(std::vector<Robot*>& allRobots) override;
    void interact(Robot& r) override;
    void move() override;
    void display() const override;
};





#endif