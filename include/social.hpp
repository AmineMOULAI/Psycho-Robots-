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
        Social(pos_t p) : Robot(RobotType::Social, p, SOCIAL_STRESS, SOCIAL_ENERGY, SOCIAL_CURIOSITY), connectedRobots{} {};
        ~Social(){};

        int get_connectedRobots() const {return connectedRobots.size();};
        
        void update(std::vector<Robot*>& allRobots) override;
        void interact(Robot& r) override;
        void move() override;
        void display() override;
};





#endif