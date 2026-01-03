#ifndef ROBOT_HPP
#define ROBOT_HPP

#include <iostream>
#include <stdbool.h>
#include "common_types.hpp"

class Robot
{
    protected:
        static int id;
        int r_id;
        RobotType type;
        pos_t pos;
        int stress, energy, curiosity;
        int sleep;

        // Target 
        bool hasTarget;
        int targetId;
        pos_t targetPos;

        // Call/Response Protocol
        RobotState state;


        //friend std::ostream& operator<<(std::ostream& out, const Robot&);
    
    public:
        // Constructor
        Robot(RobotType rname, pos_t p, int s, int e, int c) 
        : r_id(++id), type(rname), pos(p), stress(s), energy(e),
        curiosity(c), sleep(0), hasTarget(false), targetId(-1), 
        targetPos({-1, -1}), state(RobotState::Free) {};
        
        // Deconstructor
        virtual ~Robot() = default;

        // Getters
        int get_id() const {return r_id;}
        RobotType get_type() const {return type;}
        pos_t get_pos() const {return pos;}
        int get_stress() const {return stress;}
        int get_energy() const {return energy;}
        int get_curiosity() const {return curiosity;}
        int get_sleep() const {return sleep;}

        bool get_hasTarget() const {return hasTarget;}
        int get_targetId() const {return targetId;}
        pos_t get_targetPos() const {return targetPos;}

        RobotState get_state() const {return state;}

        // Setters 
        void set_hasTarget(bool h) {hasTarget = h;}
        void set_targetId(int i) {targetId = i;}
        void set_targetPos(const pos_t& p) {targetPos = p;}

        void set_state(RobotState l) {state = l;}
        void set_stress(int s) {stress = std::max(0, std::min(100, s));}
        void set_energy(int e) {energy = std::max(0, std::min(100, e));}
        void set_curiosity(int c) {curiosity = std::max(0, std::min(100, c));}
        
        // Call/Response Protocol
        virtual bool respondTo(Robot& caller) = 0;

        virtual void update(std::vector<Robot*>& allRobots);
        virtual void interact(Robot&)  = 0;
        virtual void move();
        virtual void display() const;


};






#endif