// SimUtils.h
#ifndef SIMUTILS_H
#define SIMUTILS_H
#include <string>
#include <sstream>
#include <iomanip>
#include "Train.h"
#include "VehicleEscrow.h"
#include "Locomotive.h"
#include "Carriage.h"
#include "ElectricLocomotive.h"
#include "DieselLocomotive.h"
#include "SeatCarriage.h"
#include "SleepingCarriage.h"
#include "OpenFreightCarriage.h"
#include "CoveredFreightCarriage.h"

inline std::string timeToString(int minutes)
{
    int hours = minutes / 60;
    int mins = minutes % 60;
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << hours
        << ":"
        << std::setfill('0') << std::setw(2) << mins;
    return oss.str();
}

inline std::string statusToString(TrainStatus status)
{
    switch (status)
    {
    case TrainStatus::NOT_ASSEMBLED:
        return "NOT_ASSEMBLED";
    case TrainStatus::INCOMPLETE:
        return "INCOMPLETE";
    case TrainStatus::ASSEMBLED:
        return "ASSEMBLED";
    case TrainStatus::READY:
        return "READY";
    case TrainStatus::RUNNING:
        return "RUNNING";
    case TrainStatus::ARRIVED:
        return "ARRIVED";
    case TrainStatus::FINISHED:
        return "FINISHED";
    default:
        return "UNKNOWN";
    }
}

inline std::string vehicleTypeToString(const VehiclePtr &vehicle)
{
    return std::visit([](const auto *v) -> std::string
                      {
        if (auto* loco = dynamic_cast<const Locomotive*>(v))
        {
            if (loco->getType() == LocomotiveType::Electric)
                return "Electric locomotive";
            else
                return "Diesel locomotive";
        }
        else if (auto* car = dynamic_cast<const Carriage*>(v))
        {
            switch (car->getType())
            {
            case CarriageType::Seat:          return "Seat carriage";
            case CarriageType::Sleeping:      return "Sleeping carriage";
            case CarriageType::OpenFreight:   return "Open freight carriage";
            case CarriageType::CoveredFreight:return "Covered freight carriage";
            }
        }
        return "Unknown"; }, vehicle);
}

inline int timeFromString(const std::string &time)
{
    int hours = std::stoi(time.substr(0, 2));
    int minutes = std::stoi(time.substr(3, 2));
    return hours * 60 + minutes;
}

#endif