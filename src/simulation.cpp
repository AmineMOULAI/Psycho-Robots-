#include <iostream>
#include <random>
#include <math.h>
#include <algorithm>
#include <vector>
#include <string>

#include "../include/simulation.hpp"
#include "../include/robot.hpp"
#include "../include/social.hpp"
#include "../include/worker.hpp"
#include "../include/explorer.hpp"
#include "../include/zone.hpp"
#include "../include/common_types.hpp"
#include "display_utils.hpp"






void Simulation::init() 
{
    std::cout << "Simulation initialized !" << "\n\n";

    /*std::random_device rd;
    std::mt19937 gen(rd());*/
    std::uniform_int_distribution<> dist_pos(0, G - 1);
    std::uniform_int_distribution<> dist_type(0, 2);

    int nbRobots;
    static const RobotType types[] = {
        RobotType::Social,
        RobotType::Worker,
        RobotType::Explorer
    };

    
    std::cout << "Enter number of Robots : " ;
    std::cin >> nbRobots;
    
    int nbE = 0, nbW = 0;
    // Create Robots
    for (int i = 0; i < nbRobots; i++)
    {
        pos_t p = {dist_pos(gen), dist_pos(gen)};
        const RobotType Rtype = types[dist_type(gen)];
        //std::cout << Rtype << "\n";
        switch (Rtype)
        {
            case RobotType::Social: {
                Social* s = new Social(p);
                this->robots.push_back(s);
                break;
            }
            case RobotType::Worker:{
                Worker* w = new Worker(p);
                this->robots.push_back(w);
                nbW++;
                break;
            }
            case RobotType::Explorer:{
                Explorer* e = new Explorer(p);
                this->robots.push_back(e);
                nbE++;
                break;

            }
            default:
                break;
        }
    }

    // Create Exploration Zones and Tasks

    int NbEZ = std::clamp(nbE * EZ_PER_EXPLORER, MIN_EZ, MAX_EZ); // Determine the nb of EZ based on number of Explorer robots
    int NbTasks = std::clamp(nbW * TASKS_PER_WORKER, MIN_TASKS, MAX_TASKS); // Determine the nb of Task based on number of Worker robots
    
    // Loading Tasks...
    for (int i = 0; i < NbTasks; i++)
    {
        pos_t posTask = {dist_pos(gen), dist_pos(gen)};
        Task_t* t = new Task_t{posTask, TaskStatus::Pending, {}};
        this->tasks.push_back(t);

    }

    // Loading Exploration Zones

    for (int i = 0; i < NbEZ; i++)
    {
        pos_t posEZ = {dist_pos(gen), dist_pos(gen)};
        EZ_t* ez = new EZ_t{posEZ, ZoneStatus::NotVisited, {}};
        this->explorationZone.push_back(ez);
    }
    
};

void Simulation::display_state() {
    // Restore original display sequence: title, grid, legend, zones, tasks, robots, stats
    DisplayUtils::displayTitle();
    DisplayUtils::displayGrid(G, G, robots, tasks, explorationZone);
    DisplayUtils::displayLegend();
    DisplayUtils::displayZonesInfo(explorationZone);
    DisplayUtils::displayTasksInfo(tasks);
    DisplayUtils::displayAllRobots(robots);
    DisplayUtils::displayStats(robots, explorationZone, tasks);
}

void Simulation::run_step()
{
    std::vector<Robot*> allRobots = this->get_robots();
    for (Robot* r : this->robots){
        r->update(allRobots);
        if (r->get_type() == RobotType::Explorer)
        {
            Explorer* e = dynamic_cast<Explorer*>(r);
            if (e) e->explore(this->explorationZone);
        }
        else if (r->get_type() == RobotType::Worker)
        {
            Worker* w = dynamic_cast<Worker*>(r);
            if (w && w->get_isWorking()) w->completeTask(this->tasks);
        }
    };
    
    this->step++;
    this->isSimulationComplete = this->checkSimulationComplete();
}

bool Simulation::checkSimulationComplete()
{
    int totalWorkers = 0;
    int totalExplorers = 0;
    int totalSocial = 0;

    // Count robot types
    for (Robot* r : this->robots)
    {
        if (r->get_type() == RobotType::Worker) totalWorkers++;
        else if (r->get_type() == RobotType::Explorer) totalExplorers++;
        else if (r->get_type() == RobotType::Social) totalSocial++;
    }

    // EDGE CASE 1: Only workers, no explorers/social
    if (totalWorkers > 0 && totalExplorers == 0 && totalSocial == 0)
    {
        // Check if all tasks are completed
        bool allTasksCompleted = true;
        for (Task_t* t : this->tasks)
        {
            if (t->status != TaskStatus::Completed)
            {
                allTasksCompleted = false;
                break;
            }
        }

        // If all tasks done, workers should resign
        if (allTasksCompleted)
        {
            for (Robot* r : this->robots)
            {
                if (r->get_type() == RobotType::Worker)
                {
                    Worker* w = dynamic_cast<Worker*>(r);
                    if (w && !w->get_hasResigned())
                    {
                        w->resign();
                        std::cout << "Worker " << w->get_id() << " resigned (all tasks completed).\n";
                    }
                }
            }
            if (this->step > 10)
            {
                std::cout << "[SIMULATION END] All tasks completed, all workers resigned.\n";
                return true;
            }
        }
        return false;
    }

    // EDGE CASE 2: Only explorers, no workers/social
    if (totalExplorers > 0 && totalWorkers == 0 && totalSocial == 0)
    {
        // Check if all zones are explored
        bool allZonesExplored = true;
        for (EZ_t* z : this->explorationZone)
        {
            if (z->status != ZoneStatus::Visited)
            {
                allZonesExplored = false;
                break;
            }
        }

        // If all zones explored, explorers should want to leave
        if (allZonesExplored)
        {
            for (Robot* r : this->robots)
            {
                if (r->get_type() == RobotType::Explorer)
                {
                    Explorer* e = dynamic_cast<Explorer*>(r);
                    if (e)
                    {
                        e->set_wantsNewEnvironment(true);
                        if (e->get_leaveAttempts() == 0)
                        {
                            e->set_leaveAttempts(1);
                            std::cout << "Explorer " << e->get_id() << " explored all zones and wants to leave.\n";
                        }
                    }
                }
            }
            if (this->step > 10)
            {
                std::cout << "[SIMULATION END] All zones explored, all explorers want to leave.\n";
                return true;
            }
        }
        return false;
    }

    // NORMAL CASE: Mixed robot types - check all three conditions
    
    // Check if all tasks are completed (for workers auto-resign)
    bool allTasksCompleted = true;
    for (Task_t* t : this->tasks)
    {
        if (t->status != TaskStatus::Completed)
        {
            allTasksCompleted = false;
            break;
        }
    }
    
    // If all tasks completed and we have workers, force them to resign
    if (allTasksCompleted && totalWorkers > 0)
    {
        for (Robot* r : this->robots)
        {
            if (r->get_type() == RobotType::Worker)
            {
                Worker* w = dynamic_cast<Worker*>(r);
                if (w && !w->get_hasResigned())
                {
                    w->resign();
                    std::cout << "Worker " << w->get_id() << " resigned (all tasks completed).\n";
                }
            }
        }
    }
    
    // Condition 1: Check if all workers have resigned
    bool allWorkersResigned = true;
    for (Robot* r : this->robots)
    {
        if (r->get_type() == RobotType::Worker)
        {
            Worker* w = dynamic_cast<Worker*>(r);
            if (w && !w->get_hasResigned())
            {
                allWorkersResigned = false;
            }
        }
    }
    if (totalWorkers == 0) allWorkersResigned = true;

    // Condition 2: Check if all explorers want to leave
    bool allExplorersWantToLeave = true;
    for (Robot* r : this->robots)
    {
        if (r->get_type() == RobotType::Explorer)
        {
            Explorer* e = dynamic_cast<Explorer*>(r);
            if (e && e->get_leaveAttempts() == 0)
            {
                allExplorersWantToLeave = false;
            }
        }
    }
    if (totalExplorers == 0) allExplorersWantToLeave = true;

    // Condition 3: Check if social robots have 0 interactions
    bool socialRobotsHaveZeroInteractions = true;
    for (Robot* r : this->robots)
    {
        if (r->get_type() == RobotType::Social)
        {
            Social* s = dynamic_cast<Social*>(r);
            if (s && s->get_connectedRobots() > 0)
            {
                socialRobotsHaveZeroInteractions = false;
            }
        }
    }
    if (totalSocial == 0) socialRobotsHaveZeroInteractions = true;

    // All conditions must be met AND we need at least some steps
    bool isComplete = allWorkersResigned && allExplorersWantToLeave && socialRobotsHaveZeroInteractions && this->step > 10;

    if (isComplete)
    {
        std::cout << "\n[SIMULATION COMPLETION CONDITIONS MET]\n";
        std::cout << "All workers resigned: " << (allWorkersResigned ? "YES" : "NO") << "\n";
        std::cout << "All explorers want to leave: " << (allExplorersWantToLeave ? "YES" : "NO") << "\n";
        std::cout << "Social robots have 0 interactions: " << (socialRobotsHaveZeroInteractions ? "YES" : "NO") << "\n";
        std::cout << "Total steps: " << this->step << "\n\n";
    }

    return isComplete;
}

void Simulation::displayFinalReport()
{
    std::cout << "\n\n";
    std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║          PSYCHOROBOTS FINAL SIMULATION REPORT                  ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n\n";

    std::cout << "Total Steps Executed: " << this->step << "\n\n";

    // Sort robots by type and display achievements
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "WORKERS ACHIEVEMENTS:\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    for (Robot* r : this->robots)
    {
        if (r->get_type() == RobotType::Worker)
        {
            Worker* w = dynamic_cast<Worker*>(r);
            if (w)
            {
                std::cout << "Worker " << w->get_id() << ":\n";
                std::cout << "  - Tasks Completed: " << w->get_completedTasks() << "\n";
                std::cout << "  - Points Earned: " << w->get_points() << "\n";
                std::cout << "  - Final Stress: " << w->get_stress() << "\n";
                std::cout << "  - Final Energy: " << w->get_energy() << "\n";
                std::cout << "  - Final Curiosity: " << w->get_curiosity() << "\n";
                std::cout << "\n";
            }
        }
    }

    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "EXPLORERS ACHIEVEMENTS:\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    for (Robot* r : this->robots)
    {
        if (r->get_type() == RobotType::Explorer)
        {
            Explorer* e = dynamic_cast<Explorer*>(r);
            if (e)
            {
                std::cout << "Explorer " << e->get_id() << ":\n";
                std::cout << "  - Zones Visited: " << e->get_exploredZones() << "\n";
                std::cout << "  - Final Stress: " << e->get_stress() << "\n";
                std::cout << "  - Final Energy: " << e->get_energy() << "\n";
                std::cout << "  - Final Curiosity: " << e->get_curiosity() << "\n";
                std::cout << "\n";
            }
        }
    }

    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "SOCIAL ROBOTS ACHIEVEMENTS:\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    for (Robot* r : this->robots)
    {
        if (r->get_type() == RobotType::Social)
        {
            std::cout << "Social " << r->get_id() << ":\n";
            std::cout << "  - Final Stress: " << r->get_stress() << "\n";
            std::cout << "  - Final Energy: " << r->get_energy() << "\n";
            std::cout << "  - Final Curiosity: " << r->get_curiosity() << "\n";
            std::cout << "\n";
        }
    }

    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "SIMULATION STATISTICS:\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n";

    int tasksCompleted = 0;
    for (Task_t* t : this->tasks)
    {
        if (t->status == TaskStatus::Completed) tasksCompleted++;
    }

    int zonesVisited = 0;
    for (EZ_t* z : this->explorationZone)
    {
        if (z->status == ZoneStatus::Visited) zonesVisited++;
    }

    std::cout << "Tasks Completed: " << tasksCompleted << " / " << this->tasks.size() << "\n";
    std::cout << "Zones Explored: " << zonesVisited << " / " << this->explorationZone.size() << "\n";
    std::cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                 SIMULATION END                               ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n\n";
}