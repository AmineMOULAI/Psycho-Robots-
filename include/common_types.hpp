#ifndef COMMON_TYPES
#define COMMON_TYPES

#include <random>


// Simulation Constants
constexpr int G = 10;

// Social constants
constexpr int SOCIAL_STRESS = 20;
constexpr int SOCIAL_ENERGY = 80;
constexpr int SOCIAL_CURIOSITY = 60;

// Explorer constants
constexpr int EXPLORER_STRESS = 30;
constexpr int EXPLORER_ENERGY = 90;
constexpr int EXPLORER_CURIOSITY = 90;

constexpr int EZ_PER_EXPLORER = 2;

// Worker constants
constexpr int WORKER_STRESS = 50;
constexpr int WORKER_ENERGY = 100;
constexpr int WORKER_CURIOSITY = 30;

constexpr int TASKS_PER_WORKER = 3;

// Constants 
constexpr int MIN_EZ = 4;
constexpr int MAX_EZ = 12;
constexpr int MIN_TASKS = 5;
constexpr int MAX_TASKS = 20;


// Movement const
constexpr int MOVE_COST = 2;
constexpr int REST = 20;
constexpr int REST_REWARD = 2;
constexpr int SLEEP = 15;

// Interactions Constants
constexpr double RADIUS = 5.;

constexpr double INTERACTION_DISTANCE = 1.;
constexpr int INTERACTION_TIME = 5;
constexpr int TIME_TO_WAIT = 5;

// Random 
extern std::mt19937 gen;

// Position 
struct Postion
{
    int x, y;
}typedef pos_t;

// Entites
enum class RobotType {Social, Worker, Explorer};
enum class TaskStatus {Pending, InProgress, Completed};
enum class ZoneStatus {Visited, NotVisited};
enum class Direction {N, S, E, W, NE, SE, NW, SW};

// Intercations 

struct NearbyRobot
{
    int Nid;
    double distance;
    bool SameType;    
}typedef NearbyRobot_t;

enum class CallState {Idle, Waiting, MoveTo};
enum class RobotState {Free, InCall, Busy};




 


#endif 