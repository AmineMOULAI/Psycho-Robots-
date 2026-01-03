#include "display_utils.hpp"
#include <sstream>

void DisplayUtils::clearScreen() {
    std::cout << "\033[2J\033[H";
}

void DisplayUtils::displayTitle() {
    clearScreen();
    std::cout << Colors::BOLD << Colors::BRIGHT_CYAN 
              << "╔══════════════════════════════════════════════════════════════╗\n"
              << "║         🤖 PSYCHOROBOTS - SIMULATION DISPLAY 🤖             ║\n"
              << "╚══════════════════════════════════════════════════════════════╝"
              << Colors::RESET << "\n\n";
}

void DisplayUtils::displayLegend() {
    std::cout << Colors::BRIGHT_WHITE << "\n━━━ LEGEND ━━━\n" << Colors::RESET;
    std::cout << "  " << Colors::BRIGHT_MAGENTA << "S" << Colors::RESET 
              << " = Social Robot  │  ";
    std::cout << Colors::BRIGHT_YELLOW << "W" << Colors::RESET 
              << " = Worker Robot  │  ";
    std::cout << Colors::BRIGHT_CYAN << "E" << Colors::RESET 
              << " = Explorer Robot\n";
    std::cout << "  " << Colors::GREEN << "◆" << Colors::RESET 
              << " = Exploration Zone  │  ";
    std::cout << Colors::RED << "◆" << Colors::RESET 
              << " = Task\n\n";
}

void DisplayUtils::displayGrid(int width, int height,
                               const std::vector<Robot*>& robots,
                               const std::vector<Task_t*>& tasks,
                               const std::vector<EZ_t*>& zones) {
    // Create grid with characters
    std::vector<std::vector<char>> grid(height, std::vector<char>(width, ' '));
    std::vector<std::vector<int>> gridInfo(height, std::vector<int>(width, 0));
    // 0 = empty, 1 = zone, 2 = task, 3 = robot
    
    // Place zones (Priority 1)
    for (const auto& zone : zones) {
        int x = zone->location.x;
        int y = zone->location.y;
        if (x >= 0 && x < width && y >= 0 && y < height) {
            grid[y][x] = 'Z';
            gridInfo[y][x] = 1;
        }
    }
    
    // Place tasks (Priority 2)
    for (const auto& task : tasks) {
        int x = task->location.x;
        int y = task->location.y;
        if (x >= 0 && x < width && y >= 0 && y < height && gridInfo[y][x] == 0) {
            grid[y][x] = 'T';
            gridInfo[y][x] = 2;
        }
    }
    
    // Place robots (Priority 3)
    for (const auto& robot : robots) {
        int x = robot->get_pos().x;
        int y = robot->get_pos().y;
        if (x >= 0 && x < width && y >= 0 && y < height) {
            grid[y][x] = Colors::getRobotSymbol(robot->get_type());
            gridInfo[y][x] = 3;
        }
    }
    
    // Print grid with borders
    std::cout << Colors::BRIGHT_WHITE << "╔";
    for (int i = 0; i < width * 2; ++i) std::cout << "═";
    std::cout << "╗\n" << Colors::RESET;
    
    // Print grid rows with row numbers
    for (int y = 0; y < height; ++y) {
        std::cout << Colors::BRIGHT_WHITE << "║" << Colors::RESET;
        for (int x = 0; x < width; ++x) {
            char cell = grid[y][x];
            int type = gridInfo[y][x];
            
            if (type == 3) { // Robot
                std::cout << Colors::getRobotColor(static_cast<RobotType>(cell - 'A')) << " " << cell << Colors::RESET;
            } else if (type == 1) { // Zone
                std::cout << Colors::GREEN << " ◆" << Colors::RESET;
            } else if (type == 2) { // Task
                std::cout << Colors::RED << " ◆" << Colors::RESET;
            } else {
                std::cout << "  ";
            }
        }
        std::cout << Colors::BRIGHT_WHITE << "║" << Colors::RESET;
        std::cout << " " << y << "\n";
    }
    
    // Print bottom border with column numbers
    std::cout << Colors::BRIGHT_WHITE << "╚";
    for (int i = 0; i < width * 2; ++i) std::cout << "═";
    std::cout << "╝\n" << Colors::RESET;
    
    std::cout << " ";
    for (int x = 0; x < width; ++x) {
        std::cout << " " << (x % 10);
    }
    std::cout << "\n\n";
}

std::string DisplayUtils::getZoneStatusString(ZoneStatus status) {
    switch(status) {
        case ZoneStatus::Visited: return "Visited";
        case ZoneStatus::NotVisited: return "Not Visited";
        default: return "Unknown";
    }
}

std::string DisplayUtils::getTaskStatusString(TaskStatus status) {
    switch(status) {
        case TaskStatus::Pending: return "Pending";
        case TaskStatus::InProgress: return "In Progress";
        case TaskStatus::Completed: return "Completed";
        default: return "Unknown";
    }
}

std::string DisplayUtils::getZoneStatusColor(ZoneStatus status) {
    switch(status) {
        case ZoneStatus::Visited: return Colors::GREEN;
        case ZoneStatus::NotVisited: return Colors::YELLOW;
        default: return Colors::WHITE;
    }
}

std::string DisplayUtils::getTaskStatusColor(TaskStatus status) {
    switch(status) {
        case TaskStatus::Pending: return Colors::YELLOW;
        case TaskStatus::InProgress: return Colors::CYAN;
        case TaskStatus::Completed: return Colors::GREEN;
        default: return Colors::WHITE;
    }
}

void DisplayUtils::displayZonesInfo(const std::vector<EZ_t*>& zones) {
    std::cout << Colors::BOLD << Colors::BRIGHT_CYAN << "\n[EXPLORATION ZONES]" << Colors::RESET << "\n";
    std::cout << Colors::BRIGHT_WHITE;
    for (int i = 0; i < 70; i++) std::cout << "-";
    std::cout << Colors::RESET << "\n";
    
    if (zones.empty()) {
        std::cout << "No zones available.\n";
    } else {
        int visitedCount = 0;
        for (size_t i = 0; i < zones.size(); ++i) {
            const auto& zone = zones[i];
            if (zone->status == ZoneStatus::Visited) visitedCount++;
            
            std::cout << "  Zone " << i << ": Position (" 
                      << zone->location.x << ", " << zone->location.y << ") │ "
                      << getZoneStatusColor(zone->status) 
                      << getZoneStatusString(zone->status) 
                      << Colors::RESET << " │ ";
            
            if (zone->VisitedBy.empty()) {
                std::cout << "Not visited yet";
            } else {
                std::cout << "Visited by: ";
                for (size_t j = 0; j < zone->VisitedBy.size(); ++j) {
                    std::cout << "Explorer#" << zone->VisitedBy[j];
                    if (j < zone->VisitedBy.size() - 1) std::cout << ", ";
                }
            }
            std::cout << "\n";
        }
        std::cout << "\n  Summary: " << Colors::GREEN << visitedCount << " / " 
                  << zones.size() << Colors::RESET << " zones visited\n";
    }
    std::cout << "\n";
}

void DisplayUtils::displayTasksInfo(const std::vector<Task_t*>& tasks) {
    std::cout << Colors::BOLD << Colors::BRIGHT_YELLOW << "\n[TASKS]" << Colors::RESET << "\n";
    std::cout << Colors::BRIGHT_WHITE;
    for (int i = 0; i < 70; i++) std::cout << "-";
    std::cout << Colors::RESET << "\n";
    
    if (tasks.empty()) {
        std::cout << "No tasks available.\n";
    } else {
        int pendingCount = 0, inProgressCount = 0, completedCount = 0;
        for (size_t i = 0; i < tasks.size(); ++i) {
            const auto& task = tasks[i];
            
            switch(task->status) {
                case TaskStatus::Pending: pendingCount++; break;
                case TaskStatus::InProgress: inProgressCount++; break;
                case TaskStatus::Completed: completedCount++; break;
                default: break;
            }
            
            std::cout << "  Task " << i << ": Position (" 
                      << task->location.x << ", " << task->location.y << ") │ "
                      << getTaskStatusColor(task->status) 
                      << getTaskStatusString(task->status) 
                      << Colors::RESET;
            
            if (!task->CompletedBy.empty()) {
                std::cout << " │ Completed by: ";
                for (size_t j = 0; j < task->CompletedBy.size(); ++j) {
                    std::cout << "Worker#" << task->CompletedBy[j];
                    if (j < task->CompletedBy.size() - 1) std::cout << ", ";
                }
            }
            std::cout << "\n";
        }
        std::cout << "\n  Summary: " << Colors::YELLOW << pendingCount 
                  << Colors::RESET << " pending | "
                  << Colors::CYAN << inProgressCount 
                  << Colors::RESET << " in progress | "
                  << Colors::GREEN << completedCount 
                  << Colors::RESET << " completed\n";
    }
    std::cout << "\n";
}

void DisplayUtils::displayRobotStatus(const Robot* robot) {
    std::string color = Colors::getRobotColor(robot->get_type());
    
    std::cout << "  " << color << "▶ ";
    
    switch(robot->get_type()) {
        case RobotType::Social:
            std::cout << "SOCIAL";
            break;
        case RobotType::Worker:
            std::cout << "WORKER";
            break;
        case RobotType::Explorer:
            std::cout << "EXPLORER";
            break;
        default: 
            std::cout << "UNKNOWN";
            break;
    }
    
    std::cout << Colors::RESET << " #" << robot->get_id() << "\n";
    std::cout << "    Position: (" << robot->get_pos().x << ", " 
              << robot->get_pos().y << ")\n";
    
    // Health bar for energy
    std::cout << "    Energy:    ";
    displayHealthBar(robot->get_energy(), 100, 15);
    std::cout << "\n";
    
    // Stress indicator
    std::cout << "    Stress:    ";
    displayStressBar(robot->get_stress(), 100, 15);
    std::cout << "\n";
    
    // Curiosity bar
    std::cout << "    Curiosity: ";
    displayCuriosityBar(robot->get_curiosity(), 100, 15);
    std::cout << "\n";
    
    if (robot->get_type() == RobotType::Social) {
        robot->display();
    }
}

void DisplayUtils::displayAllRobots(const std::vector<Robot*>& robots) {
    if (robots.empty()) {
        std::cout << "No robots in simulation.\n";
        return;
    }
    
    std::cout << Colors::BOLD << Colors::BRIGHT_WHITE << "\n[ROBOT DETAILS]" << Colors::RESET << "\n";
    std::cout << Colors::BRIGHT_WHITE;
    for (int i = 0; i < 70; i++) std::cout << "-";
    std::cout << Colors::RESET << "\n";
    
    for (const auto& robot : robots) {
        displayRobotStatus(robot);
    }
}

void DisplayUtils::displayStats(const std::vector<Robot*>& robots,
                                const std::vector<EZ_t*>& zones,
                                const std::vector<Task_t*>& tasks) {
    std::cout << Colors::BOLD << Colors::BRIGHT_GREEN << "\n[SIMULATION STATISTICS]" << Colors::RESET << "\n";
    std::cout << Colors::BRIGHT_WHITE;
    for (int i = 0; i < 70; i++) std::cout << "=";
    std::cout << Colors::RESET << "\n\n";
    
    // Robot counts
    int socialCount = 0, workerCount = 0, explorerCount = 0;
    for (const Robot* r : robots) {
        switch(r->get_type()) {
            case RobotType::Social: socialCount++; break;
            case RobotType::Worker: workerCount++; break;
            case RobotType::Explorer: explorerCount++; break;
            default: break;
        }
    }
    
    std::cout << "ROBOTS:\n";
    std::cout << "  " << Colors::BRIGHT_MAGENTA << "Social Robots: " << Colors::RESET 
              << socialCount << "\n";
    std::cout << "  " << Colors::BRIGHT_YELLOW << "Worker Robots: " << Colors::RESET 
              << workerCount << "\n";
    std::cout << "  " << Colors::BRIGHT_CYAN << "Explorer Robots: " << Colors::RESET 
              << explorerCount << "\n";
    std::cout << "  Total: " << robots.size() << "\n\n";
    
    // Zones info
    int visitedZones = 0;
    for (const auto& zone : zones) {
        if (zone->status == ZoneStatus::Visited) visitedZones++;
    }
    std::cout << "EXPLORATION ZONES:\n";
    std::cout << "  Total Zones: " << zones.size() << "\n";
    std::cout << "  " << Colors::GREEN << "Visited: " << Colors::RESET << visitedZones << "\n";
    std::cout << "  " << Colors::YELLOW << "Not Visited: " << Colors::RESET 
              << (zones.size() - visitedZones) << "\n\n";
    
    // Tasks info
    int pendingTasks = 0, inProgressTasks = 0, completedTasks = 0;
    for (const auto& task : tasks) {
        switch(task->status) {
            case TaskStatus::Pending: pendingTasks++; break;
            case TaskStatus::InProgress: inProgressTasks++; break;
            case TaskStatus::Completed: completedTasks++; break;
            default: break;
        }
    }
    std::cout << "TASKS:\n";
    std::cout << "  Total Tasks: " << tasks.size() << "\n";
    std::cout << "  " << Colors::YELLOW << "Pending: " << Colors::RESET << pendingTasks << "\n";
    std::cout << "  " << Colors::CYAN << "In Progress: " << Colors::RESET << inProgressTasks << "\n";
    std::cout << "  " << Colors::GREEN << "Completed: " << Colors::RESET << completedTasks << "\n";
    
    std::cout << "\n" << Colors::BRIGHT_WHITE;
    for (int i = 0; i < 70; i++) std::cout << "=";
    std::cout << Colors::RESET << "\n\n";
}

void DisplayUtils::displayHealthBar(int current, int max, int width) {
    int filled = (current * width) / max;
    std::cout << Colors::BRIGHT_GREEN << "[";
    for (int i = 0; i < width; ++i) {
        if (i < filled) std::cout << "█";
        else std::cout << "░";
    }
    std::cout << "] " << current << "/" << max << Colors::RESET;
}

void DisplayUtils::displayStressBar(int current, int max, int width) {
    int filled = (current * width) / max;
    std::cout << Colors::RED << "[";
    for (int i = 0; i < width; ++i) {
        if (i < filled) std::cout << "█";
        else std::cout << "░";
    }
    std::cout << "] " << current << "/" << max << Colors::RESET;
}

void DisplayUtils::displayCuriosityBar(int current, int max, int width) {
    int filled = (current * width) / max;
    std::cout << Colors::BRIGHT_YELLOW << "[";
    for (int i = 0; i < width; ++i) {
        if (i < filled) std::cout << "█";
        else std::cout << "░";
    }
    std::cout << "] " << current << "/" << max << Colors::RESET;
}

void DisplayUtils::displaySplitLayout(int width, int height,
                                      const std::vector<Robot*>& robots,
                                      const std::vector<Task_t*>& tasks,
                                      const std::vector<EZ_t*>& zones,
                                      int leftWidth) {
    // Print title once (clears screen)
    DisplayUtils::displayTitle();

    // Capture right (grid) output
    std::ostringstream rightSS;
    std::streambuf* oldBuf = std::cout.rdbuf(rightSS.rdbuf());
    DisplayUtils::displayGrid(width, height, robots, tasks, zones);
    std::cout.rdbuf(oldBuf);

    // Capture left (info) output
    std::ostringstream leftSS;
    oldBuf = std::cout.rdbuf(leftSS.rdbuf());
    DisplayUtils::displayLegend();
    DisplayUtils::displayZonesInfo(zones);
    DisplayUtils::displayTasksInfo(tasks);
    DisplayUtils::displayAllRobots(robots);
    DisplayUtils::displayStats(robots, zones, tasks);
    std::cout.rdbuf(oldBuf);

    // Split into lines
    auto splitLines = [](const std::string &s){
        std::vector<std::string> lines;
        std::istringstream iss(s);
        std::string line;
        while (std::getline(iss, line)) lines.push_back(line);
        return lines;
    };

    std::vector<std::string> L = splitLines(leftSS.str());
    std::vector<std::string> R = splitLines(rightSS.str());

    size_t total = std::max(L.size(), R.size());
    for (size_t i = 0; i < total; ++i) {
        std::string leftLine = (i < L.size()) ? L[i] : std::string("");
        if ((int)leftLine.size() < leftWidth) leftLine += std::string(leftWidth - leftLine.size(), ' ');
        else if ((int)leftLine.size() > leftWidth) leftLine = leftLine.substr(0, leftWidth);

        std::string rightLine = (i < R.size()) ? R[i] : std::string("");

        std::cout << leftLine << "  " << rightLine << "\n";
    }
}
