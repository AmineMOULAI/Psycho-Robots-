#ifndef WORKER_HPP
#define WORKER_HPP

#include <iostream>
#include <vector>
#include "robot.hpp"
#include "common_types.hpp"
#include "zone.hpp"

class Worker : public Robot
{
    private:
        int completedTasks;
        bool hasResigned;
        int points;
        bool isWorking;
    public:
        Worker(pos_t p) : Robot(RobotType::Worker, p, WORKER_STRESS, WORKER_ENERGY, WORKER_CURIOSITY), completedTasks(0), hasResigned(false), points(0), isWorking(true) {};
        ~Worker(){};

        int get_completedTasks() const {return completedTasks;};
        bool get_hasResigned() const {return hasResigned;};
        int get_points() const {return points;};
        bool get_isWorking() const {return isWorking;};

        void resign() {hasResigned = true; isWorking = false;};
        void completeTask(std::vector<Task_t*>& tasks);
        void gainPoints(int p) {points += p;};

        // Call/Response Protocol
        bool respondTo(Robot& caller) override;

        void update(std::vector<Robot*>& allRobots) override;
        void interact(Robot& r) override;
        void move() override;
        void display() const override;
};





#endif