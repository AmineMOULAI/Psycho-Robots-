#include "../include/worker.hpp"
#include "common_types.hpp"

bool Worker::respondTo(Robot& caller)
{
    return this->get_curiosity() > 40 && this->get_stress() < 60 && this->get_energy() > 30;
}

void Worker::update(std::vector<Robot*>& allRobots) { Robot::update(allRobots);};
void Worker::interact(Robot& r) { 
    std::cout << "Worker interacts with ";
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
void Worker::move(){Robot::move();};
void Worker::display() const
{
    Robot::display();
    std::cout << "Completed tasks : " << this->get_completedTasks() << "\n\n";
};