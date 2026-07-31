// Simulation.cpp
#include <vector>
#include <queue>
#include <memory>
#include <fstream>
#include "../include/SimUtils.h"
#include "../include/Simulation.h"
#include "../include/events/Event.h"
#include "../include/events/EventQueue.h"
#include "../include/events/AssembleEvent.h"

Simulation::Simulation(std::vector<std::unique_ptr<Train>> trains,
                       std::vector<std::unique_ptr<Station>> stations,
                       std::vector<Track> tracks) : trains_(std::move(trains)), stations_(std::move(stations)), tracks_(std::move(tracks)), currentTime_(0)
{
    initializeEvents();
    logFile_.open("Trainsim.log");
}

Simulation::~Simulation() { logFile_.close(); }

Station *Simulation::findStation(const std::string &name)
{
    auto it = std::ranges::find_if(
        stations_,
        [&name](const auto &station)
        {
            return station->getStationName() == name;
        });

    return it != stations_.end() ? it->get() : nullptr;
}

void Simulation::scheduleEvent(std::unique_ptr<Event> event)
{
    queue_.pushEvent(std::move(event));
}

void Simulation::initializeEvents()
{
    for (auto &train : trains_)
    {
        auto depStation = findStation(train->getDepartureStation());

        if (depStation == nullptr)
            throw std::runtime_error("Station not found");

        scheduleEvent(
            std::make_unique<AssembleEvent>(
                *train,
                train->getScheduledDepartureTime() - 30,
                *depStation,
                vehicleEscrow_));
    }
}

void Simulation::step(int interval)
{
    int targetTime = currentTime_ + interval;
    while (!queue_.empty() && queue_.topTime() <= targetTime)
    {
        currentTime_ = queue_.topTime();

        if (currentTime_ >= 1440)
        {
            queue_.removeIfInactive();
            if (queue_.empty() || queue_.topTime() > targetTime)
                break;
        }

        processNextEvent();
    }
    currentTime_ = targetTime;
}

void Simulation::stepToNextEvent()
{
    if (!queue_.empty())
    {
        currentTime_ = queue_.topTime();
        processNextEvent();
    }
}

void Simulation::processNextEvent()
{
    auto event = queue_.pop();

    TrainStatus oldStatus = event->getTrain().getStatus();

    auto next = event->processEvent();
    TrainStatus newStatus = event->getTrain().getStatus();

    if (next == nullptr && newStatus == TrainStatus::ARRIVED)
    {
        Station *arrStation = findStation(event->getTrain().getArrivalStation());
        if (arrStation == nullptr)
            throw std::runtime_error("Arrival station not found");
        scheduleEvent(std::move(std::make_unique<FinishEvent>(
            event->getTrain(),
            event->getTime() + 20,
            *arrStation,
            vehicleEscrow_)));
    }
    else if (newStatus == TrainStatus::FINISHED)
    {
        statistics_.emplace_back(event->getTrain().getTrainNumber(),
                                 true,
                                 event->getTrain().getDelay() == 0 ? true : false,
                                 event->getTrain().getDelay(),
                                 event->getTrain().getActualArrivalTime() - event->getTrain().getScheduledArrivalTime());
    }
    else if (next != nullptr)
    {
        scheduleEvent(std::move(next));
    }

    eventLog_.emplace_back(
        currentTime_,
        event->getTrain().getTrainNumber(),
        event->getTrain().getDepartureStation(),
        event->getTrain().getArrivalStation(),
        event->getTrain().getScheduledDepartureTime(),
        event->getTrain().getScheduledArrivalTime(),
        event->getTrain().getActualArrivalTime(),
        event->getTrain().getDelay(),
        oldStatus,
        newStatus);

    writeToLog(eventLog_.back());
}

const std::vector<SimEvent> &Simulation::getEventLog() const
{
    return eventLog_;
}

const std::vector<TrainStats> &Simulation::getTrainStats() const
{
    return statistics_;
}

void Simulation::clearEventLog()
{
    eventLog_.clear();
}

int Simulation::getCurrentTime() const
{
    return currentTime_;
}

const std::vector<std::unique_ptr<Train>> &Simulation::getTrains() const
{
    return trains_;
}

const Train *Simulation::getTrainByNumber(int trainNumber) const
{
    auto it = std::ranges::find(trains_, trainNumber, &Train::getTrainNumber);

    if (it != trains_.end())
        return it->get();

    return nullptr;
}

const Station *Simulation::getStationByName(const std::string &stationName) const
{
    auto it = std::ranges::find_if(stations_,
                                   [&stationName](const auto &s)
                                   {
                                       return s->getStationName() == stationName;
                                   });

    if (it != stations_.end())
        return it->get();

    return nullptr;
}

const VehicleLocation Simulation::findVehicleById(int vehicleId) const
{
    auto stationIt = std::ranges::find_if(stations_, [vehicleId](const auto &station)
                                          { return station->getLocomotiveById(vehicleId) != nullptr ||
                                                   station->getCarriageById(vehicleId) != nullptr; });

    if (stationIt != stations_.end())
        return VehicleLocation{true, false, 0, (*stationIt)->getStationName(), TrainStatus::NOT_ASSEMBLED};

    for (const auto &train : trains_)
    {
        auto trainIt = std::ranges::find_if(train->getVehicleSequence(), [vehicleId](const VehiclePtr &vehicle)
                                            { return std::visit([vehicleId](const auto *ptr)
                                                                { return ptr->getId() == vehicleId; }, vehicle); });
        if (trainIt != train->getVehicleSequence().end())
            return VehicleLocation{true, true, train->getTrainNumber(), "", train->getStatus()};
    }
    return VehicleLocation{false, false, 0, "", TrainStatus::NOT_ASSEMBLED};
}

void Simulation::writeToLog(const SimEvent &event)
{
    logFile_ << timeToString(event.time) << " Train " << event.trainNumber
             << ": " << statusToString(event.oldStatus)
             << " -> " << statusToString(event.newStatus) << "\n";
    logFile_.flush();
}

bool Simulation::isFinished()
{
    if (currentTime_ >= 1440)
    {
        if (!statisticsFinalized_)
        {
            statisticsFinalized_ = true;
            for (const auto &train : trains_)
            {
                if (train->getStatus() == TrainStatus::INCOMPLETE || train->getStatus() == TrainStatus::NOT_ASSEMBLED)
                {
                    statistics_.emplace_back(train->getTrainNumber(),
                                             false,
                                             false,
                                             train->getDelay(),
                                             train->getActualArrivalTime() - train->getScheduledArrivalTime());
                }
            }
        }
        for (const auto &train : trains_)
        {
            if (train->getStatus() == TrainStatus::RUNNING || train->getStatus() == TrainStatus::ARRIVED)
                return false;
        }
    }
    else
    {
        for (const auto &train : trains_)
        {
            if (train->getStatus() != TrainStatus::FINISHED)
                return false;
        }
    }

    return true;
}