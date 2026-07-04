// Station.h
#ifndef STATION_H
#define STATION_H

#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include "Locomotive.h"
#include "Carriage.h"

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
    Locomotive *getLocomotiveById(int id);
    std::unique_ptr<Locomotive> extractLocomotiveById(int id);

    const std::vector<std::unique_ptr<Carriage>> &getCarriages() const;
    Carriage *getCarriageById(int id);
    std::unique_ptr<Carriage> extractCarriageById(int id);

    std::string getStationName();
};

#endif
