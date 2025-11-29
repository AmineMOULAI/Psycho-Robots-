#include <algorithm>

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
    std::uniform_int_distribution<> dist_dir(0, 3);
    
    static const Direction d[] = {
        Direction::UP, 
        Direction::DOWN, 
        Direction::LEFT, 
        Direction::RIGHT
    };

    Direction dir = d[dist_dir(gen)];
    switch (dir)
    {
        case Direction::UP:{
            if (this->pos.y > 0) this->pos.y--; 
            break;
        }
            
        case Direction::DOWN:{
            if (this->pos.y < G - 1) this->pos.y++; 
            break;
        }
        case Direction::LEFT:{
            if (this->pos.x > 0) this->pos.x--; 
            break;

        }
        case Direction::RIGHT:{
            if (this->pos.x < G - 1) this->pos.x++; 
            break;

        }
        default:
            break;
    }   
}