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
        Worker(pos_t p) : Robot(RobotType::Worker, p, 50, 100, 30), completedTasks(0) {};
        ~Worker(){};

        int get_completedTasks() const {return completedTasks;};

        void update() override;
        void interact(Robot& r) override;
        void move() override;
        void display() override;
};





#endif