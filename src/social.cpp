#include "../include/social.hpp"
#include "common_types.hpp"

void Social::update() { Robot::update();};
void Social::interact(Robot& r) { 
    std::cout << "Social interacts with "; 
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
    std::cout << r.get_id() << " !\n";
};

void Social::move(){ Robot::move();};

void Social::display()
{
    Robot::display();
    std::cout << "Connected robots : " << this->get_connectedRobots() << "\n\n";
};