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

std::string Station::getStationName() { return stationName_; }