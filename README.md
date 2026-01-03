# Psycho-Robots 🤖

PsychoRobots is a C++ console simulation of autonomous robots with distinct psychological states (stress, curiosity, energy) that pursue individual goals on a 2D grid.

## Overview

Three robot types operate in a dynamic simulation:
- **Worker**: Completes tasks, manages stress, resigns when done
- **Explorer**: Discovers zones, shares knowledge, leaves when curiosity depletes
- **Social**: Interacts with other robots to decrease stress

## Key Features

- 30x30 2D grid environment with random placement
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

## Recent Updates (Jan 3, 2026)

✅ Worker task completion and resignation logic
✅ Explorer curiosity depletion triggers leave
✅ Stats clamping (0-100 range)
✅ Automatic simulation completion detection
✅ Bug fixes for premature exits

## Project Structure

```
include/  - Header files (robot types, simulation, utilities)
src/      - Implementation files
run.cpp   - Main entry point
Makefile  - Build configuration
```

## Configuration

Key constants in `common_types.hpp`:
- Grid: 30x30
- Tasks/Worker: 3
- Zones/Explorer: 2
- Interaction distance: 1.0

## Future

- Persistence (save/load)
- Advanced AI learning
- Team cooperation
- GUI visualization
