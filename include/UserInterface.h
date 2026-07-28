#ifndef USERINTERFACE_H
#define USERINTERFACE_H
#include "Simulation.h"

class UserInterface
{
private:
    Simulation &sim_;
    int interval_ = 10;

    void showMainMenu();
    void showInfoMenu();
    void showTrainDetails(int trainNumber);
    void showStationDetails(const std::string &stationName);
    void printCurrentTime() const;
    void printEventLog() const;
    void printStatistics() const;

    void printTimetable() const;
    void showTrainDetails(int id) const;
    void showStationDetails(int name) const;
    void findVehicleById(int vehicleId) const;

public:
    UserInterface(const UserInterface &) = delete;
    UserInterface &operator=(const UserInterface &) = delete;

    UserInterface(Simulation &sim) : sim_(sim) {}
    void run();
};

#endif