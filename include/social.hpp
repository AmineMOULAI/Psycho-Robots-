#ifndef SOCIAL_HPP
#define SOCIAL_HPP

#include <iostream>
#include <vector>
#include "robot.hpp"
#include "common_types.hpp"

class Social : public Robot
{
    private:
        std::vector<int> connectedRobots;
    public:
        Social(pos_t p) : Robot(RobotType::Social, p, 20, 80, 60), connectedRobots{} {};
        ~Social(){};

        int get_connectedRobots() const {return connectedRobots.size();};
        
        void update() override;
        void interact(Robot& r) override;
        void move() override;
        void display() override;
};





#endif