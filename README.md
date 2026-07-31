# Trains Simulation

Discrete event-driven train simulation engine built in modern C++23.
A reimplementation of the DT060G university assignment (originally 
completed 2019), rebuilt from scratch to demonstrate professional-level 
C++ practices.

## Tech Stack
- C++23 — simulation engine
- GoogleTest — unit testing
- CMake 3.20+

## Features
- Discrete event-driven simulation of a full day of train traffic
- Configurable start and end time
- Electric and diesel locomotives with unique characteristics
- Passenger, sleeping and freight carriages
- Vehicle pool management with ownership transfer between stations
- Departure and arrival schedule simulation with delay tracking
- Data-driven: reads trains, stations and map data from files

## Building
```bash
mkdir build && cd build
cmake .. && make
```

## Running
```bash
./trains
```

## Testing
```bash
cd build && ctest
```

## Design
See [DESIGN_DECISIONS.md](docs/DESIGN_DECISIONS.md) for architecture 
and key design decisions.