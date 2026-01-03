# Psycho-Robots 🤖

PsychoRobots is a C++ console simulation of autonomous robots with distinct psychological states (stress, curiosity, energy) that pursue individual goals on a 2D grid.

## Overview

Three robot types operate in a dynamic simulation:
- **Worker**: Completes tasks, manages stress, resigns when done
- **Explorer**: Discovers zones, shares knowledge, leaves when curiosity depletes
- **Social**: Interacts with other robots to decrease stress

## Key Features

- NxN 2D grid environment with random placement
- Task system (pending/completed)
- Exploration zones to discover
- Real-time ASCII visualization with detailed statistics
- CALL/RESPONSE interaction protocol
- Automatic simulation completion detection
- Comprehensive final report

## Robot Attributes

| Stat | Range | Use |
|------|-------|-----|
| Stress | 0-100 | Decreases with interaction |
| Energy | 0-100 | Consumed by movement, restored by rest |
| Curiosity | 0-100 | Drives exploration and interaction |

## Building & Running

```bash
make              # Compile
./bin/run         # Run simulation
make clean        # Clean build files
```

## Simulation End Conditions

**Workers Only**: All tasks completed → workers resign → end
**Explorers Only**: All zones visited → explorers leave → end
**Mixed Robots**: All resign + all explorers leave + socials isolated (0 interactions) → end


## Project Structure

```
include/  - Header files (robot types, simulation, utilities)
src/      - Implementation files
run.cpp   - Main entry point
Makefile  - Build configuration
```

## Future

- Persistence (save/load)
- Advanced AI learning
- Team cooperation
- GUI visualization

### Planned Features

**Graphics & Visualization**
- OpenGL/SDL2 graphical visualization
- 3D environment rendering
- Real-time animation of robot movements
- Interactive visual debugger

**New Robot Types**
- Soldiers/Guards: Protective units, patrol areas
- Traders: Exchange resources, merchant activity
- Teachers: Transfer knowledge to other robots
- Scouts: Quick explorers with lower energy cost

**Communication System**
- Robot-to-robot chat protocol
- Message broadcasting
- Information sharing system
- Gossip/rumor spreading mechanics

**Environment Enhancements**
- Dynamic obstacles (static/moving)
- Multiple environment levels/maps
- Portal system for zone transitions
- Dynamically generated environments

**Economy System**
- Boss/Company entities that hire workers
- Salary and payment system
- Job contracts and negotiations
- Worker skill progression
- Career advancement mechanics

**Learning & Training**
- Virtual labs for robot behavior training
- Machine learning integration
- Reinforcement learning for optimal strategies
- Genetic algorithms for population evolution

**Activity Expansion**
- Trading posts and markets
- Resource gathering and management
- Colony building mechanics
- Team formation and roles
- Hierarchy and leadership systems
- Achievement/quest system

**Advanced Mechanics**
- Conflict resolution between robots
- Cooperation bonuses for group work
- Reputation system
- Aging and robot lifecycle
- Reproduction/robot creation
- Emotional states beyond stress/curiosity
- Memory and historical tracking

# TOO BE CONTINUED...
