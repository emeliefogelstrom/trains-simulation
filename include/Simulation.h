// Simulation.h
#ifndef SIMULATION_H
#define SIMULATION_H
#include <vector>
#include <queue>
#include <memory>
#include "Station.h"
#include "Train.h"
#include "VehicleEscrow.h"
#include "Track.h"
#include "./events/Event.h"

struct EventComparator
{
    bool operator()(const std::unique_ptr<Event> &a,
                    const std::unique_ptr<Event> &b) const
    {
        if (a->getTime() != b->getTime())
            return a->getTime() > b->getTime();
        return a->getTrainNumber() > b->getTrainNumber();
    }
};

class Simulation
{
private:
    VehicleEscrow vehicleEscrow_;
    std::vector<std::unique_ptr<Train>> trains_;
    std::vector<std::unique_ptr<Station>> stations_;
    std::vector<Track> tracks_;
    std::priority_queue<
        std::unique_ptr<Event>,
        std::vector<std::unique_ptr<Event>>,
        EventComparator>
        queue_;
    int currentTime_;

    Station *findStation(const std::string &name);
    void scheduleEvent(std::unique_ptr<Event> event);
    void initializeEvents();

public:
    Simulation(std::vector<std::unique_ptr<Train>> trains,
               std::vector<std::unique_ptr<Station>> stations,
               std::vector<Track> tracks);

    void step(int interval);
    void runSimulation();
};
#endif