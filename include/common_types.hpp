#ifndef COMMON_TYPES
#define COMMON_TYPES

#include <random>

// Constants 
constexpr int G = 20;
constexpr int MIN_EZ = 4;
constexpr int MAX_EZ = 12;
constexpr int EZ_PER_EXPLORER = 2;
constexpr int MIN_TASKS = 5;
constexpr int MAX_TASKS = 20;
constexpr int TASKS_PER_WORKER = 3;

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
 


#endif 