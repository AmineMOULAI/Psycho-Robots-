#ifndef DISPLAY_UTILS_HPP
#define DISPLAY_UTILS_HPP

#include <iostream>
#include <string>
#include <vector>
#include "robot.hpp"
#include "common_types.hpp"
#include "zone.hpp"

// ANSI Color Codes
namespace Colors {
    // Foreground Colors
    const std::string RESET = "\033[0m";
    const std::string BOLD = "\033[1m";
    const std::string DIM = "\033[2m";
    
    // Basic Colors
    const std::string BLACK = "\033[30m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string WHITE = "\033[37m";
    
    // Bright Colors
    const std::string BRIGHT_RED = "\033[91m";
    const std::string BRIGHT_GREEN = "\033[92m";
    const std::string BRIGHT_YELLOW = "\033[93m";
    const std::string BRIGHT_BLUE = "\033[94m";
    const std::string BRIGHT_MAGENTA = "\033[95m";
    const std::string BRIGHT_CYAN = "\033[96m";
    const std::string BRIGHT_WHITE = "\033[97m";
    
    // Background Colors
    const std::string BG_RED = "\033[41m";
    const std::string BG_GREEN = "\033[42m";
    const std::string BG_YELLOW = "\033[43m";
    const std::string BG_BLUE = "\033[44m";
    const std::string BG_MAGENTA = "\033[45m";
    const std::string BG_CYAN = "\033[46m";
    const std::string BG_WHITE = "\033[47m";
    
    // Get color for robot type
    inline std::string getRobotColor(RobotType type) {
        switch(type) {
            case RobotType::Social: return BRIGHT_MAGENTA;
            case RobotType::Worker: return BRIGHT_YELLOW;
            case RobotType::Explorer: return BRIGHT_CYAN;
            default: return WHITE;
        }
    }
    
    // Get symbol for robot type
    inline char getRobotSymbol(RobotType type) {
        switch(type) {
            case RobotType::Social: return 'S';
            case RobotType::Worker: return 'W';
            case RobotType::Explorer: return 'E';
            default: return '?';
        }
    }
}

// Display Utilities
class DisplayUtils {
public:
    // Screen management
    static void clearScreen();
    
    // Main display functions
    static void displayTitle();
    static void displayLegend();
    static void displayGrid(int width, int height, 
                           const std::vector<Robot*>& robots,
                           const std::vector<Task_t*>& tasks,
                           const std::vector<EZ_t*>& zones);
    
    // Robot information
    static void displayRobotStatus(const Robot* robot);
    static void displayAllRobots(const std::vector<Robot*>& robots);
    
    // Zones and tasks information
    static void displayZonesInfo(const std::vector<EZ_t*>& zones);
    static void displayTasksInfo(const std::vector<Task_t*>& tasks);
    
    // Statistics
    static void displayStats(const std::vector<Robot*>& robots,
                            const std::vector<EZ_t*>& zones,
                            const std::vector<Task_t*>& tasks);

private:
    // Bar displays
    static void displayHealthBar(int current, int max, int width);
    static void displayStressBar(int current, int max, int width);
    static void displayCuriosityBar(int current, int max, int width);
    
    // Helper functions
    static std::string getZoneStatusString(ZoneStatus status);
    static std::string getTaskStatusString(TaskStatus status);
    static std::string getZoneStatusColor(ZoneStatus status);
    static std::string getTaskStatusColor(TaskStatus status);
};

#endif
