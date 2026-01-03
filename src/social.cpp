#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

#include "social.hpp"
#include "common_types.hpp"
#include "interactions_utils.hpp"
#include "robot.hpp"


bool Social::respondTo(Robot& caller)
{
    return this->get_curiosity() > 50 && this->get_stress() < 50 && this->get_energy() > 30;
}

void Social::attemptCall(Robot* target)
{
    if (!target) return;

    bool response = target->respondTo(*this);
    if (response)
    {
        std::cout << "CALL ACCEPTED by " << target->get_id() << std::endl;
        
        target->set_state(RobotState::InCall);
        set_socialState(CallState::MoveTo);
        set_interactionTimer(INTERACTION_TIME);
    }
    else{ 
        std::cout << "CALL REJECTED by " << target->get_id() << std::endl;
        set_socialState(CallState::Waiting);
        set_waitTimer(TIME_TO_WAIT);
        set_hasTarget(false);
    } 
}

void Social::handle_call_protocol(Robot* target)
{
    if (get_socialState() == CallState::Idle) return;
    if (get_socialState() == CallState::Waiting)
    {
        int t = get_waitTimer();
        if (t > 0) set_waitTimer(t - 1);
        else set_socialState(CallState::Idle);
    }
    if (get_socialState() == CallState::MoveTo)
    {
        double d = get_distance(get_pos(), target->get_pos());
        if(d <= INTERACTION_DISTANCE)
        {
            std::cout << "Social " << get_id() << " REACHED " << target->get_id() << std::endl;
            connectedRobots.push_back(target->get_id());

            int it = get_interactionTimer();
            if (it > 0) set_interactionTimer(it - 1);
            else{
                target->set_state(RobotState::Free);
    
                set_socialState(CallState::Idle);
                set_hasTarget(false);
            }
            
        }
        else set_targetPos(target->get_pos());
    }
}    


void Social::update(std::vector<Robot*>& allRobots) 
{ 
    if(get_socialState() == CallState::Idle && !get_hasTarget())
    {
        NearbyRobot_t nearby = buildAndSortNRobots(*this, allRobots);
        std::vector<int>::iterator f = std::find(connectedRobots.begin(), connectedRobots.end(), get_targetId());
        if (nearby.Nid != -1 && (f == connectedRobots.end() || *f != nearby.Nid)) 
        {
            set_hasTarget(true);
            set_targetId(nearby.Nid);
            set_targetPos(allRobots[nearby.Nid - 1]->get_pos());
    
            attemptCall(allRobots[nearby.Nid - 1]);
        }
        
    }
    if((get_hasTarget() && get_socialState() != CallState::Idle) || get_socialState() == CallState::Waiting)
        handle_call_protocol(allRobots[get_targetId() - 1]);
        
    Robot::update(allRobots);

};



void Social::interact(Robot& r) { 
    std::cout << "Social interacts with "; 
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

void Social::move(){
    
    if (get_socialState() == CallState::MoveTo)
    {
        int x = get_pos().x, xt = get_targetPos().x;
        int y = get_pos().y, yt = get_targetPos().y;
        if(abs(x - xt) > 1 || abs(y - yt) > 1)
        {
            if (x < xt && y < yt) { x++; y++; } 
            if (x < xt && y > yt) { x++; y--; }
            if (x > xt && y < yt) { x--; y++; }
            if (x > xt && y > yt) { x--; y--; }
            if (x == xt && y < yt) y++;
            if (x == xt && y > yt) y--;
            if (x < xt && y == yt) x++;
            if (x > xt && y == yt) x--;

            this->pos.x = x; this->pos.y = y; 
        }
    }else Robot::move();
};

void Social::display() const 
{
    /*Robot::display();
    std::cout << "Connected robots : " << this->get_connectedRobots() << "\n\n";
    std::cout << "Target : ";
    if (this->get_hasTarget() == true) std::cout << "1, ";
    else std::cout << "0, ";
    std::cout << "id : " << this->get_targetId() << "\n\n";*/
    if (this->get_hasTarget())
        std::cout << "call established with Robot: " << this->get_targetId() << std::endl;
    if (this->get_socialState() == CallState::MoveTo)
        std::cout << "Interaction : " << this->get_interactionTimer() << std::endl;
    

};