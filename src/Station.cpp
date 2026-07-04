#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include "../include/Locomotive.h"
#include "../include/Carriage.h"
#include "../include/Station.h"

Station::Station(std::string name) : stationName_(name) {}

void Station::addLocomotive(std::unique_ptr<Locomotive> locomotive)
{
    locomotives_.push_back(std::move(locomotive));
}

void Station::addCarriage(std::unique_ptr<Carriage> carriage)
{
    carriages_.push_back(std::move(carriage));
}

const std::vector<std::unique_ptr<Locomotive>> &Station::getLocomotives() const { return locomotives_; }

Locomotive *Station::getLocomotiveById(int id)
{
    auto it = std::ranges::find(locomotives_, id, &Locomotive::getId);

    if (it == locomotives_.end())
    {
        return nullptr;
    }

    return it->get();
}

std::unique_ptr<Locomotive> Station::extractLocomotiveById(int id)
{
    auto it = std::ranges::find_if(locomotives_, [id](const auto &locomotive)
                                   { return locomotive->getId() == id; });

    if (it == locomotives_.end())
    {
        return nullptr;
    }

    auto result = std::move(*it);
    locomotives_.erase(it);
    return result;
}

const std::vector<std::unique_ptr<Carriage>> &Station::getCarriages() const { return carriages_; }

Carriage *Station::getCarriageById(int id)
{
    auto it = std::ranges::find(carriages_, id, &Carriage::getId);

    if (it == carriages_.end())
    {
        return nullptr;
    }
    return it->get();
}

std::unique_ptr<Carriage> Station::extractCarriageById(int id)
{
    auto it = std::ranges::find_if(carriages_, [id](const auto &carriage)
                                   { return carriage->getId() == id; });

    if (it == carriages_.end())
    {
        return nullptr;
    }

    auto result = std::move(*it);
    carriages_.erase(it);
    return result;
}

std::string Station::getStationName()
{
    return stationName_;
}