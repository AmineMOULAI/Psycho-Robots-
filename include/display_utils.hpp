#ifndef DISPLAY_UTILS_HPP
#define DISPLAY_UTILS_HPP

#include <iostream>
#include <string>
#include <vector>
#include "robot.hpp"
#include "common_types.hpp"
#include "simulation.hpp"


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
            case RobotType::Social: return BRIGHT_MAGENTA;    // Magenta pour Social
            case RobotType::Worker: return BRIGHT_YELLOW;     // Jaune pour Worker
            case RobotType::Explorer: return BRIGHT_CYAN;     // Cyan pour Explorer
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
    static void clearScreen() {
        std::cout << "\033[2J\033[H";  // Clear screen + move cursor to top
    }
    
    static void displayGrid(int width, int height, 
                           const std::vector<Robot*>& robots,
                           const std::vector<Task_t*>& tasks,
                           const std::vector<EZ_t*>& zones) {
        // Create grid
        std::vector<std::string> grid(height, std::string(width, ' '));
        
        // Place zones (Priority 1 - lowest)
        for (size_t i = 0; i < zones.size(); ++i) {
            // You'll need to cast properly based on your Task/Zone structure
            // For now, placeholder
        }
        
        // Place tasks (Priority 2)
        for (size_t i = 0; i < tasks.size(); ++i) {
            // Placeholder
        }
        
        // Place robots (Priority 3 - highest)
        for (Robot* robot : robots) {
            pos_t p = robot->get_pos();
            if (p.x >= 0 && p.x < width && p.y >= 0 && p.y < height) {
                grid[p.y][p.x] = Colors::getRobotSymbol(robot->get_type());
            }
        }
        
        // Print border top
        std::cout << Colors::BRIGHT_WHITE << "╔";
        for (int i = 0; i < width * 2; ++i) std::cout << "=";
        std::cout << "╗" << Colors::RESET << std::endl;
        
        // Print grid with colors
        for (int y = 0; y < height; ++y) {
            std::cout << Colors::BRIGHT_WHITE << "║" << Colors::RESET;
            for (int x = 0; x < width; ++x) {
                char cell = grid[y][x];
                
                if (cell == 'S') {
                    std::cout << Colors::BRIGHT_MAGENTA << " S" << Colors::RESET;
                } else if (cell == 'W') {
                    std::cout << Colors::BRIGHT_YELLOW << " W" << Colors::RESET;
                } else if (cell == 'E') {
                    std::cout << Colors::BRIGHT_CYAN << " E" << Colors::RESET;
                } else if (cell == 'T') {
                    std::cout << Colors::RED << " T" << Colors::RESET;
                } else if (cell == 'Z') {
                    std::cout << Colors::GREEN << " Z" << Colors::RESET;
                } else {
                    std::cout << "  ";
                }
            }
            std::cout << Colors::BRIGHT_WHITE << "║" << Colors::RESET << std::endl;
        }
        
        // Print border bottom
        std::cout << Colors::BRIGHT_WHITE << "╚";
        for (int i = 0; i < width * 2; ++i) std::cout << "=";
        std::cout << "╝" << Colors::RESET << std::endl;
    }
    
    static void displayRobotStatus(const Robot* robot) {
        std::string color = Colors::getRobotColor(robot->get_type());
        
        std::cout << color << "▶ " << Colors::RESET;
        
        switch(robot->get_type()) {
            case RobotType::Social:
                std::cout << color << "SOCIAL" << Colors::RESET;
                break;
            case RobotType::Worker:
                std::cout << color << "WORKER" << Colors::RESET;
                break;
            case RobotType::Explorer:
                std::cout << color << "EXPLORER" << Colors::RESET;
                break;
            default: break;
        }
        
        std::cout << " #" << robot->get_id() << "\n";
        std::cout << "  Position: (" << robot->get_pos().x << "," 
                  << robot->get_pos().y << ")\n";
        
        // Health bar for energy
        std::cout << "  Energy: ";
        displayHealthBar(robot->get_energy(), 100, 20);
        std::cout << "\n";
        
        // Stress indicator
        std::cout << "  Stress:  ";
        displayStressBar(robot->get_stress(), 100, 20);
        std::cout << "\n";
        
        // Curiosity bar
        std::cout << "  Curiosity: ";
        displayCuriosityBar(robot->get_curiosity(), 100, 20);
        std::cout << "\n";
        
        if (robot->get_type() == RobotType::Social) {
            robot->display();
        }       
        
        std::cout << "\n";
    }
    
    static void displayStats(const std::vector<Robot*>& robots) {
        std::cout << Colors::BRIGHT_WHITE << "\n" << std::string(60, '=') 
                  << Colors::RESET << "\n";
        std::cout << Colors::BOLD << "SIMULATION STATS" << Colors::RESET << "\n";
        std::cout << Colors::BRIGHT_WHITE << std::string(60, '=') 
                  << Colors::RESET << "\n\n";
        
        int socialCount = 0, workerCount = 0, explorerCount = 0;
        for (const Robot* r : robots) {
            switch(r->get_type()) {
                case RobotType::Social: socialCount++; break;
                case RobotType::Worker: workerCount++; break;
                case RobotType::Explorer: explorerCount++; break;
                default: break;
            }
        }
        
        std::cout << Colors::BRIGHT_MAGENTA << "Social Robots: " << Colors::RESET 
                  << socialCount << "\n";
        std::cout << Colors::BRIGHT_YELLOW << "Worker Robots: " << Colors::RESET 
                  << workerCount << "\n";
        std::cout << Colors::BRIGHT_CYAN << "Explorer Robots: " << Colors::RESET 
                  << explorerCount << "\n";
        
        std::cout << "\n";
    }

private:
    static void displayHealthBar(int current, int max, int width) {
        int filled = (current * width) / max;
        std::cout << Colors::BRIGHT_GREEN << "[";
        for (int i = 0; i < width; ++i) {
            if (i < filled) std::cout << "█";
            else std::cout << "░";
        }
        std::cout << "] " << current << "/" << max << Colors::RESET;
    }
    
    static void displayStressBar(int current, int max, int width) {
        int filled = (current * width) / max;
        std::cout << Colors::RED << "[";
        for (int i = 0; i < width; ++i) {
            if (i < filled) std::cout << "█";
            else std::cout << "░";
        }
        std::cout << "] " << current << "/" << max << Colors::RESET;
    }
    
    static void displayCuriosityBar(int current, int max, int width) {
        int filled = (current * width) / max;
        std::cout << Colors::BRIGHT_YELLOW << "[";
        for (int i = 0; i < width; ++i) {
            if (i < filled) std::cout << "█";
            else std::cout << "░";
        }
        std::cout << "] " << current << "/" << max << Colors::RESET;
    }
};

#endif
