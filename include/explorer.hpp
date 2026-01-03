#ifndef EXPLORER_HPP
#define EXPLORER_HPP

#include <iostream>
#include <vector>
#include "robot.hpp"
#include "common_types.hpp"
#include "zone.hpp"

class Explorer : public Robot
{
    private:
        std::vector<pos_t> exploredZones;
        std::vector<EZ_t*> knownZones;
        bool wantsNewEnvironment;
        int leaveAttempts;
    public:
        Explorer(pos_t p) : Robot(RobotType::Explorer, p, EXPLORER_STRESS, EXPLORER_ENERGY, EXPLORER_CURIOSITY), exploredZones{}, knownZones{}, wantsNewEnvironment(false), leaveAttempts(0) {};
        ~Explorer(){};

    int get_exploredZones() const {return exploredZones.size();};
    int get_leaveAttempts() const {return leaveAttempts;};
    bool get_wantsNewEnvironment() const { return wantsNewEnvironment; }
    const std::vector<EZ_t*>& get_knownZones() const { return knownZones; }

    bool respondTo(Robot& caller) override;
    void update(std::vector<Robot*>& allRobots) override;
    void interact(Robot& r) override;
    void move() override;
    void display() const override;
    void explore(std::vector<EZ_t*>& explorationZones);
};

#endif