#include <algorithm>
#include <unistd.h>

#include "../include/robot.hpp"
#include "../include/social.hpp"
#include "../include/worker.hpp"
#include "../include/explorer.hpp"
#include "common_types.hpp"

int Robot::id = 0;

void Robot::display()
{
    std::cout << "Name : ";
    switch (this->get_type())
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
    std::cout << this->get_id() << "\n"
    << "Position : " << "(" << this->get_pos().x << "," << this->get_pos().y <<")"<< "\n" 
    << "Stress : " << this->get_stress() << "\n" 
    << "Energy : " << this->get_energy() << "\n" 
    << "Curiosity : " << this->get_curiosity() << "\n";
}

void Robot::move()
{
    std::uniform_int_distribution<> dist_pos(0, G - 1);
    std::uniform_int_distribution<> dist_dir(0, 7);
    
    static const Direction d[] = {
        Direction::N, 
        Direction::S, 
        Direction::E, 
        Direction::W,
        Direction::NE, 
        Direction::SE, 
        Direction::NE, 
        Direction::SW,
    };

    Direction dir = d[dist_dir(gen)];
    switch (dir)
    {
        case Direction::N:{
            if (this->pos.y > 0) this->pos.y--; 
            break;
        }
        case Direction::S:{
            if (this->pos.y < G - 1) this->pos.y++; 
            break;
        }
        case Direction::E:{
            if (this->pos.x < G - 1) this->pos.x++; 
            break;
        }
        case Direction::W:{
            if (this->pos.x > 0) this->pos.x--; 
            break;
        }
        case Direction::NE:{
            if (this->pos.y > 0 && this->pos.x < G - 1)
            {
                this->pos.x++; 
                this->pos.y--; 
            } 
            break;
        }     
        case Direction::SE:{
            if (this->pos.x < G - 1 && this->pos.y < G - 1)
            {
                this->pos.x++; 
                this->pos.y++; 
            } 
            break;
        }
        case Direction::NW:{
            if (this->pos.x > 0 && this->pos.y > 0)
            {
                this->pos.x--; 
                this->pos.y--; 
            }
            break;

        }
        case Direction::SW:{
            if (this->pos.x > 0 && this->pos.y < G - 1)
            {
                this->pos.x--; 
                this->pos.y++;
            } 
            break;
        }
        default:
            break;
    }   
}

void Robot::update()
{

    if(this->get_energy() >= REST && this->get_sleep() == 0)
    {
        this->move();
        this->energy -= MOVE_COST;
        this->sleep = 0;
    }else 
    {
        if (this->get_sleep() <= SLEEP)
        {
            this->energy += REST_REWARD;
            this->sleep++;
        }else{this->sleep = 0;}
    }
}