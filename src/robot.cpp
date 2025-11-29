#include "../include/robot.hpp"
#include "../include/social.hpp"
#include "../include/worker.hpp"
#include "../include/explorer.hpp"
#include "common_types.hpp"

int Robot::id = 0;

void Robot::display()
{
    std::cout << "Name : ";
    switch (this->get_type())
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
    std::cout << this->get_id() << "\n"
    << "Position : " << "(" << this->get_pos().x << "," << this->get_pos().y <<")"<< "\n" 
    << "Stress : " << this->get_stress() << "\n" 
    << "Energy : " << this->get_energy() << "\n" 
    << "Curiosity : " << this->get_curiosity() << "\n";
}