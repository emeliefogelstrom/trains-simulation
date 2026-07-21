// Simulation.cpp
#include <iostream>
#include <vector>
#include <queue>
#include <memory>
#include "../include/Simulation.h"
#include "../include/events/Event.h"
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
    queue_.push(std::move(event));
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
    while (!queue_.empty() && queue_.top()->getTime() <= currentTime_)
    {
        // priority_queue::top() returns a const ref, which prevents std::move.
        // const_cast is the standard workaround when using unique_ptr in a priority_queue.
        auto event = std::move(const_cast<std::unique_ptr<Event> &>(queue_.top()));
        queue_.pop();
        auto next = event->processEvent();
        if (next == nullptr && event->getTrain().getStatus() == TrainStatus::ARRIVED)
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
        else
        {
            scheduleEvent(std::move(next));
        }
    }
}