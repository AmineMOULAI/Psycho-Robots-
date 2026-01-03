#ifndef WORKER_HPP
#define WORKER_HPP

#include <iostream>
#include <vector>
#include "robot.hpp"
#include "common_types.hpp"

class Worker : public Robot
{
    private:
        int completedTasks;
    public:
        Worker(pos_t p) : Robot(RobotType::Worker, p, WORKER_STRESS, WORKER_ENERGY, WORKER_CURIOSITY), completedTasks(0) {};
        ~Worker(){};

        int get_completedTasks() const {return completedTasks;};

        // Call/Response Protocol
        bool respondTo(Robot& caller) override;

        void update(std::vector<Robot*>& allRobots) override;
        void interact(Robot& r) override;
        void move() override;
        void display() const override;
};





#endif