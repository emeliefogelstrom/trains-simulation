// Train.h
#ifndef TRAIN_H
#define TRAIN_H
#include "Station.h"
#include "VehicleEscrow.h"

enum class TrainStatus
{
    NOT_ASSEMBLED,
    INCOMPLETE,
    ASSEMBLED,
    READY,
    RUNNING,
    ARRIVED,
    FINISHED
};

using VehiclePtr = std::variant<const Locomotive *, const Carriage *>;

class Train
{
private:
    TrainStatus status_;
    std::vector<VehiclePtr> vehicleSequence_;
    std::vector<int> requiredVehicleTypes_;

    int trainNumber_;
    std::string departureStation_;
    std::string arrivalStation_;

    int scheduledDepartureTime_;
    int scheduledArrivalTime_;
    int maxSpeed_;
    int distance_;
    int delay_;

public:
    Train(int trainNumber, const std::string &departureStation,
          const std::string &arrivalStation, int scheduledDepartureTime,
          int scheduledArrivalTime, int maxSpeed, int distance,
          const std::vector<int> &requiredVehicleTypes);
    bool tryAssemble(Station &departure, VehicleEscrow &box);
    void ready();
    void depart();
    void arrive();
    void finish(Station &arrival, VehicleEscrow &box);

    int getTrainNumber() const;
    std::string getDepartureStation() const;
    std::string getArrivalStation() const;

    TrainStatus getStatus() const;
    const std::vector<VehiclePtr> &getVehicleSequence() const;
    const std::vector<int> &getRequiredVehicleTypes() const;

    int getMaxSpeed() const;
    int getScheduledDepartureTime() const;
    int getScheduledArrivalTime() const;
    double getAverageSpeed() const;
    void addDelay(int delay);
    int getDelay() const;
};

#endif