#ifndef ROBOT_HPP
#define ROBOT_HPP

#include <iostream>
#include "common_types.hpp"

class Robot
{
    protected:
        static int id;
        int r_id;
        RobotType type;
        pos_t pos;
        int stress, energy, curiosity;

        //friend std::ostream& operator<<(std::ostream& out, const Robot&);
    
    public:
        // Constructor
        Robot(RobotType rname, pos_t p, int s, int e, int c) : r_id(++id), type(rname), pos(p), stress(s), energy(e), curiosity(c){};
        virtual ~Robot() = default;

        // Getters
        int get_id() const {return r_id;};
        RobotType get_type() const {return type;};
        pos_t get_pos() const {return pos;};
        int get_stress() const {return stress;};
        int get_energy() const {return energy;};
        int get_curiosity() const {return curiosity;};

        virtual void update();
        virtual void interact(Robot&)  = 0;
        virtual void move();
        virtual void display();

};






#endif