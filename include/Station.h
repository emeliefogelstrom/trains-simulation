// Station.h
#ifndef STATION_H
#define STATION_H

#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <optional>
#include "Locomotive.h"
#include "Carriage.h"
#include "LocomotiveType.h"

class Station
{
private:
    std::vector<std::unique_ptr<Locomotive>> locomotives_;
    std::vector<std::unique_ptr<Carriage>> carriages_;
    std::string stationName_;

public:
    Station(std::string name);
    void addLocomotive(std::unique_ptr<Locomotive> locomotive);
    void addCarriage(std::unique_ptr<Carriage> carriage);
    const std::vector<std::unique_ptr<Locomotive>> &getLocomotives() const;
    Locomotive *getLocomotiveById(int id) const;
    std::optional<int> getLowestIdByLocomotiveType(LocomotiveType type, const std::vector<int> &claimedIds) const;
    std::unique_ptr<Locomotive> extractLocomotiveById(int id);

    const std::vector<std::unique_ptr<Carriage>> &getCarriages() const;
    Carriage *getCarriageById(int id) const;
    std::optional<int> getLowestIdByCarriageType(CarriageType type, const std::vector<int> &claimedIds) const;
    std::unique_ptr<Carriage> extractCarriageById(int id);

    std::string getStationName() const;
};

#endif
