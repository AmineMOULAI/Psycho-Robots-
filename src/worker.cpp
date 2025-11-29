#include "../include/worker.hpp"
#include "common_types.hpp"

void Worker::update() { return;};
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
void Worker::move(){std::cout << "Worker moves !" << "\n";};
void Worker::display()
{
    std::cout << "Worker introduces itself !" << "\n";
    Robot::display();
    std::cout << "Completed tasks : " << this->get_completedTasks() << "\n\n";
};