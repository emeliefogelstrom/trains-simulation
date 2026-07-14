#include "../include/Train.h"
#include "../include/Station.h"
#include "../include/VehicleEscrow.h"
#include "../include/VehicleTypeUtils.h"
#include <iostream>
#include <map>

Train::Train(int trainNumber, const std::string &departureStation,
             const std::string &arrivalStation, int scheduledDepartureTime,
             int scheduledArrivalTime, int maxSpeed, int distance,
             const std::vector<int> &requiredVehicleTypes) : trainNumber_(trainNumber), departureStation_(departureStation),
                                                             arrivalStation_(arrivalStation), scheduledDepartureTime_(scheduledDepartureTime),
                                                             scheduledArrivalTime_(scheduledArrivalTime), maxSpeed_(maxSpeed), distance_(distance),
                                                             requiredVehicleTypes_(requiredVehicleTypes), status_(TrainStatus::NOT_ASSEMBLED) {}

bool Train::tryAssemble(Station &departure, VehicleEscrow &box)
{
    enum class Kind
    {
        Loco,
        Car
    };

    std::vector<std::pair<Kind, int>> vehiclesToExtract;
    std::vector<int> claimedIds;

    for (auto &type : requiredVehicleTypes_)
    {
        if (VehicleTypeUtils::isLocomotive(type))
        {
            auto locoType = VehicleTypeUtils::convertLocomotiveType(type);
            auto locoId = departure.getLowestIdByLocomotiveType(locoType, claimedIds);
            if (!locoId)
            {
                status_ = TrainStatus::INCOMPLETE;
                return false;
            }
            vehiclesToExtract.emplace_back(Kind::Loco, *locoId);
            claimedIds.push_back(*locoId);
        }
        else
        {
            auto carType = VehicleTypeUtils::convertCarriageType(type);
            auto carId = departure.getLowestIdByCarriageType(carType, claimedIds);
            if (!carId)
            {
                status_ = TrainStatus::INCOMPLETE;
                return false;
            }
            vehiclesToExtract.emplace_back(Kind::Car, *carId);
            claimedIds.push_back(*carId);
        }
    }

    for (const auto &vehicle : vehiclesToExtract)
    {
        if (vehicle.first == Kind::Loco)
        {
            auto extractLoc = departure.extractLocomotiveById(vehicle.second);
            vehicleSequence_.push_back(extractLoc.get());
            box.add(std::move(extractLoc));
        }
        else
        {
            auto extractCar = departure.extractCarriageById(vehicle.second);
            vehicleSequence_.push_back(extractCar.get());
            box.add(std::move(extractCar));
        }
    }

    status_ = TrainStatus::ASSEMBLED;
    return true;
}

void Train::markReady() { status_ = TrainStatus::READY; }

void Train::depart() { status_ = TrainStatus::RUNNING; }

void Train::arrive() { status_ = TrainStatus::ARRIVED; }

void Train::finish(Station &arrival, VehicleEscrow &box)
{
    for (const auto &vehicle : vehicleSequence_)
    {
        if (auto loco = std::get_if<const Locomotive *>(&vehicle))
        {
            auto extracted = box.extract((*loco)->getId());
            arrival.addLocomotive(
                std::move(std::get<std::unique_ptr<Locomotive>>(extracted)));
        }
        else if (auto car = std::get_if<const Carriage *>(&vehicle))
        {

            auto extracted = box.extract((*car)->getId());
            arrival.addCarriage(std::move(std::get<std::unique_ptr<Carriage>>(extracted)));
        }
    }
    status_ = TrainStatus::FINISHED;
}

int Train::getTrainNumber() const { return trainNumber_; }

int Train::getMaxSpeed() const { return maxSpeed_; }

std::string Train::getDepartureStation() const { return departureStation_; }

std::string Train::getArrivalStation() const { return arrivalStation_; }

TrainStatus Train::getStatus() const { return status_; }
const std::vector<VehiclePtr> &Train::getVehicleSequence() const { return vehicleSequence_; }
const std::vector<int> &Train::getRequiredVehicleTypes() const { return requiredVehicleTypes_; }

double Train::getAverageSpeed() const
{
    double travelTime = scheduledArrivalTime_ - scheduledDepartureTime_;
    double hours = travelTime / 60.0;
    return distance_ / hours;
}