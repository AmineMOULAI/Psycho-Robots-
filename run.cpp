#include <iostream>
#include <random>
#include <unistd.h>

#include "robot.hpp"
#include "social.hpp"
#include "worker.hpp"
#include "explorer.hpp"
#include "simulation.hpp"
#include "zone.hpp"
#include "common_types.hpp"



int main()
{
    /*std::random_device rd;
    std::mt19937 gen(rd());*/
    std::uniform_int_distribution<> dist(0, G - 1);

    std::cout << "############## PSYCHOROBOTS PROJECT #############\n\n";
    /*pos_t p1 = {1, 1};
    Robot social("Social", p1, 30, 60, 50);
    std::cout << social;
    pos_t p2 = {3, 1};
    Robot worker("Worker", p2, 60, 80, 20);
    std::cout << worker;
    
    pos_t p1 = {dist(gen), dist(gen)};
    Social s1(p1);
    //s1.display();

    pos_t p2 = {1, 5};
    Worker w1(p2);
    //w1.display();

    
    pos_t p3 = {2, 1};
    Explorer e1(p3);
    //e1.display();

    //s1.interact(e1);
    //w1.interact(s1);
    */
    Simulation game(G, G);
    
    game.init();
    while(!game.get_isComplete())
    {
        game.run_step();
        system("clear");
        game.display_state();

        sleep(1);
        
    }
    
    // Display final report after simulation ends
    game.displayFinalReport();
    
    return 0;
}