// Simulation.h
#ifndef SIMULATION_H
#define SIMULATION_H
#include <vector>
#include <memory>
#include <fstream>
#include "Station.h"
#include "Train.h"
#include "VehicleEscrow.h"
#include "Track.h"
#include "./events/Event.h"
#include "./events/EventQueue.h"

struct SimEvent
{
    int time;
    int trainNumber;
    std::string departureStation;
    std::string arrivalStation;
    int scheduledDepartureTime;
    int scheduledArrivalTime;
    int delay;
    TrainStatus oldStatus;
    TrainStatus newStatus;
};

struct VehicleLocation
{
    bool found;
    bool onTrain;
    int trainNumber;
    std::string stationName;
    TrainStatus trainStatus;
};

struct TrainStats
{
    int trainNumber;
    bool departed;
    bool onTime;
    int departureDelay;
    int arrivalDelay;
};

class Simulation
{
private:
    VehicleEscrow vehicleEscrow_;
    EventQueue queue_;
    std::vector<std::unique_ptr<Train>> trains_;
    std::vector<std::unique_ptr<Station>> stations_;
    std::vector<Track> tracks_;
    std::vector<SimEvent> eventLog_;
    std::vector<TrainStats> statistics_;
    int currentTime_;
    std::ofstream logFile_;
    bool statisticsFinalized_ = false;

    Station *
    findStation(const std::string &name);
    void scheduleEvent(std::unique_ptr<Event> event);
    void initializeEvents();
    void writeToLog(const SimEvent &event);

public:
    Simulation(const Simulation &) = delete;
    Simulation &operator=(const Simulation &) = delete;
    Simulation(std::vector<std::unique_ptr<Train>> trains,
               std::vector<std::unique_ptr<Station>> stations,
               std::vector<Track> tracks);
    ~Simulation();

    void step(int interval);
    void stepToNextEvent();
    void processNextEvent();
    const std::vector<SimEvent> &getEventLog() const;
    const std::vector<TrainStats> &getTrainStats() const;
    void clearEventLog();
    int getCurrentTime() const;
    const std::vector<std::unique_ptr<Train>> &getTrains() const;
    const Train *getTrainByNumber(int trainNumber) const;
    const Station *getStationByName(const std::string &stationName) const;
    const VehicleLocation findVehicleById(int vehicleId) const;
    bool isFinished();
};
#endif