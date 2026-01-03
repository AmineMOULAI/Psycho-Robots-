#ifndef SOCIAL_HPP
#define SOCIAL_HPP

#include <iostream>
#include <vector>
#include "robot.hpp"
#include "common_types.hpp"

class Social : public Robot
{
    private:
        std::vector<int> connectedRobots;
        CallState socialState;
        int waitTimer;
        int interactionTimer;

    public:
        Social(pos_t p) : 
        Robot(RobotType::Social, p, SOCIAL_STRESS, SOCIAL_ENERGY, SOCIAL_CURIOSITY), 
        connectedRobots{}, socialState(CallState::Idle), waitTimer(0), interactionTimer(0) {};
        
        ~Social(){};
        // Getters 
        int get_connectedRobots() const {return connectedRobots.size();};
        CallState get_socialState() const {return socialState;}
        int get_waitTimer() const {return waitTimer;}
        int get_interactionTimer() const {return interactionTimer;}

        // Setters
        void set_socialState(CallState state) {socialState = state;}
        void set_waitTimer(int wt) {waitTimer = wt;}
        void set_interactionTimer(int it) {interactionTimer = it;}

        // Call/Response Protocol
        bool respondTo(Robot& caller) override;

        void attemptCall(Robot* target);
        void handle_call_protocol(Robot* target);

        // Methods
        void update(std::vector<Robot*>& allRobots) override;
        void interact(Robot& r) override;
        void move() override;
        void display() const override;
};





#endif