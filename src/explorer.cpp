#include "../include/explorer.hpp"
#include "common_types.hpp"

void Explorer::update() { Robot::update();};
void Explorer::interact(Robot& r) 
{ 
    std::cout << "Explorer interacts with "; 
    switch (r.get_type())
    {
    case RobotType::Social:
        std::cout << "Social\n";
        break;
    case RobotType::Worker:
        std::cout << "Worker\n";
        break;
    case RobotType::Explorer:
        std::cout << "Explorer\n";
        break;

    default:
        break;
    } 
    std::cout << r.get_id() << " !\n";
};
void Explorer::move(){ Robot::move();};
void Explorer::display()
{
    Robot::display();
    std::cout << "Explored Zones : " << this->get_exploredZones() << "\n\n";
};