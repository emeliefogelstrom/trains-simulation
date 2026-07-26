// Simulation.cpp
#include <vector>
#include <queue>
#include <memory>
#include "../include/Simulation.h"
#include "../include/events/Event.h"
#include "../include/events/EventQueue.h"
#include "../include/events/AssembleEvent.h"

Simulation::Simulation(std::vector<std::unique_ptr<Train>> trains,
                       std::vector<std::unique_ptr<Station>> stations,
                       std::vector<Track> tracks) : trains_(std::move(trains)), stations_(std::move(stations)), tracks_(std::move(tracks)), currentTime_(0) { initializeEvents(); }

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
    currentTime_ += interval;
    runSimulation();
}

void Simulation::runSimulation()
{
    while (!queue_.empty() && queue_.topTime() <= currentTime_)
    {
        processNextEvent();
    }
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
        event->getTrain().getDelay(),
        oldStatus,
        newStatus);
}

const std::vector<SimEvent> &Simulation::getEventLog() const
{
    return eventLog_;
}

void Simulation::clearEventLog()
{
    eventLog_.clear();
}

int Simulation::getCurrentTime() const
{
    return currentTime_;
}