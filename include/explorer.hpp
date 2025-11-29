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
        Explorer(pos_t p) : Robot(RobotType::Explorer, p, 30, 90, 90), exploredZones{} {};
        ~Explorer(){};

    int get_exploredZones() const {return exploredZones.size();};

    void update() override;
    void interact(Robot& r) override;
    void move() override;
    void display() override;
};





#endif