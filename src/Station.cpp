#include <vector>
#include <memory>
#include <algorithm>
#include <optional>
#include "../include/Locomotive.h"
#include "../include/Carriage.h"
#include "../include/Station.h"
#include "../include/LocomotiveType.h"

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

Locomotive *Station::getLocomotiveById(int id) const
{
    auto it = std::ranges::find(locomotives_, id, &Locomotive::getId);

    if (it == locomotives_.end())
    {
        return nullptr;
    }

    return it->get();
}

std::optional<int> Station::getLowestIdByLocomotiveType(LocomotiveType type, const std::vector<int> &claimedIds) const
{
    std::optional<int> lowest_id;

    for (const auto &loco : locomotives_)
    {
        if (loco->getType() != type)
            continue;

        if (std::ranges::contains(claimedIds, loco->getId()))
            continue;

        if (!lowest_id || loco->getId() < *lowest_id)
            lowest_id = loco->getId();
    }

    return lowest_id;
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

Carriage *Station::getCarriageById(int id) const
{
    auto it = std::ranges::find(carriages_, id, &Carriage::getId);

    if (it == carriages_.end())
    {
        return nullptr;
    }
    return it->get();
}

std::optional<int> Station::getLowestIdByCarriageType(CarriageType type, const std::vector<int> &claimedIds) const
{
    std::optional<int> lowest_id;

    for (const auto &car : carriages_)
    {
        if (car->getType() != type)
            continue;

        if (std::ranges::contains(claimedIds, car->getId()))
            continue;

        if (!lowest_id || car->getId() < *lowest_id)
            lowest_id = car->getId();
    }

    return lowest_id;
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

std::string Station::getStationName() const
{
    return stationName_;
}