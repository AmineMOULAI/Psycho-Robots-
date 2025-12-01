#include "../include/worker.hpp"
#include "common_types.hpp"

void Worker::update() { Robot::update();};
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
void Worker::display()
{
    Robot::display();
    std::cout << "Completed tasks : " << this->get_completedTasks() << "\n\n";
};