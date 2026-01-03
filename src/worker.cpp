#include "../include/worker.hpp"
#include "../include/interactions_utils.hpp"
#include "common_types.hpp"
#include <cmath>

bool Worker::respondTo(Robot& caller)
{
    return this->get_curiosity() > 40 && this->get_stress() < 60 && this->get_energy() > 30;
}

void Worker::completeTask(std::vector<Task_t*>& tasks)
{
    if (!this->isWorking || this->get_energy() < 10) return;

    // Find nearest pending task
    Task_t* nearestTask = nullptr;
    double minDistance = std::numeric_limits<double>::max();

    for (size_t i = 0; i < tasks.size(); i++)
    {
        if (tasks[i]->status == TaskStatus::Pending)
        {
            double distance = get_distance(this->pos, tasks[i]->location);
            if (distance < minDistance)
            {
                minDistance = distance;
                nearestTask = tasks[i];
            }
        }
    }

    // If at task location, complete it
    if (nearestTask && minDistance <= INTERACTION_DISTANCE)
    {
        // Complete the task
        nearestTask->status = TaskStatus::Completed;
        nearestTask->CompletedBy.push_back(this->r_id);
        this->completedTasks++;

        // Gain points
        this->gainPoints(10);

        // Decrease stress after completing task
        this->stress = std::max(0, this->stress - 15);

        // Gain curiosity after completing task
        this->curiosity = std::min(100, this->curiosity + 5);

        // Consume energy
        this->energy -= 10;
    }
    else if (nearestTask && minDistance < 30)
    {
        // Move towards the task if not too far
        if (this->pos.x < nearestTask->location.x) this->pos.x++;
        else if (this->pos.x > nearestTask->location.x) this->pos.x--;

        if (this->pos.y < nearestTask->location.y) this->pos.y++;
        else if (this->pos.y > nearestTask->location.y) this->pos.y--;

        this->energy -= MOVE_COST;
    }
}

void Worker::update(std::vector<Robot*>& allRobots)
{
    if (this->hasResigned) return;

    // If no energy, resign
    if (this->get_energy() <= 0)
    {
        this->resign();
        return;
    }

    Robot::update(allRobots);

    // Workers focus on tasks rather than socializing
    if (this->get_energy() > 30 && this->get_state() == RobotState::Free)
    {
        // Try to work (handled in completeTask method called from simulation)
        this->isWorking = true;
    }
}

void Worker::interact(Robot& r)
{
    // Workers try to transmit task info when interacting with social robots
    if (r.get_type() == RobotType::Social && this->completedTasks > 0)
    {
        std::cout << "Worker " << this->r_id << " shares task knowledge with Social " << r.get_id() << "!\n";
    }
    else
    {
        std::cout << "Worker " << this->r_id << " interacts with ";
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
    }
}

void Worker::move()
{
    Robot::move();
}

void Worker::display() const
{
    Robot::display();
    std::cout << "Completed tasks : " << this->get_completedTasks() << "\n"
              << "Points : " << this->get_points() << "\n"
              << "Status : " << (this->hasResigned ? "Resigned" : "Working") << "\n\n";
}