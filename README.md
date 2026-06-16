# Trains Simulation

Real-time train simulation engine built in modern C++20 with WebSocket 
streaming and React dashboard.

## Tech Stack
- C++20 — simulation engine
- Boost.Asio — WebSocket server
- PostgreSQL — persistent data
- React — real-time dashboard

## Features
- Train traffic simulation in real time
- Electric and diesel locomotives, holding unique characteristics
- Multiple carriages including passenger carriage, sleeping carriage and freigt carriage
- Real time updates via WebSocket to React dashboard
- Departure and arrival shedule simulation
- Data driven. Reads trains, stations and map data from files


## Building
```bash
mkdir build && cd build
cmake .. && make
```

## Running
```bash
./trains
```