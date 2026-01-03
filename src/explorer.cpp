#include "../include/explorer.hpp"
#include "common_types.hpp"


bool Explorer::respondTo(Robot& caller)
{
    return this->get_curiosity() > 50 && this->get_energy() > 20;
}


void Explorer::update(std::vector<Robot*>& allRobots) { Robot::update(allRobots);};
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
void Explorer::display() const
{
    Robot::display();
    std::cout << "Explored Zones : " << this->get_exploredZones() << "\n\n";
};