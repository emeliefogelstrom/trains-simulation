#include "../include/Train.h"
#include "../include/Station.h"
#include "../include/VehicleEscrow.h"

Train::Train(int trainNumber, const std::string &departureStation,
             const std::string &arrivalStation, int scheduledDepartureTime,
             int scheduledArrivalTime, int maxSpeed,
             const std::vector<int> &requiredVehicleTypes) : trainNumber_(trainNumber), departureStation_(departureStation),
                                                             arrivalStation_(arrivalStation), scheduledDepartureTime_(scheduledDepartureTime),
                                                             scheduledArrivalTime_(scheduledArrivalTime), maxSpeed_(maxSpeed),
                                                             requiredVehicleTypes_(requiredVehicleTypes), status_(TrainStatus::NOT_ASSEMBLED) {}

// Fixa
bool Train::tryAssemble(Station &departure, VehicleEscrow &box)
{
    status_ = TrainStatus::ASSEMBLED;
    return true;
}
void Train::markReady() { status_ = TrainStatus::READY; }

void Train::depart() { status_ = TrainStatus::RUNNING; }

void Train::arrive() { status_ = TrainStatus::ARRIVED; }

// Fixa
void Train::finish(Station &arrival, VehicleEscrow &box)
{
    status_ = TrainStatus::FINISHED;
}

int Train::getTrainNumber() const { return trainNumber_; }

std::string Train::getDepartureStation() const { return departureStation_; }

std::string Train::getArrivalStation() const { return arrivalStation_; }

TrainStatus Train::getStatus() const { return status_; }
const std::vector<VehiclePtr> &Train::getVehicleSequence() const { return vehicleSequence_; }
const std::vector<int> &Train::getRequiredVehicleTypes() const { return requiredVehicleTypes_; }