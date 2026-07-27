// UserInterface.cpp
#include "../include/UserInterface.h"
#include "../include/Train.h"
#include "../include/ElectricLocomotive.h"
#include "../include/DieselLocomotive.h"
#include "../include/SeatCarriage.h"
#include "../include/SleepingCarriage.h"
#include "../include/CoveredFreightCarriage.h"
#include "../include/OpenFreightCarriage.h"
#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include <algorithm>

void UserInterface::run()
{
    printCurrentTime();

    bool running = true;
    while (running)
    {

        std::cout << "\n=== MAIN MENU ===\n"
                  << "1. Step forward " << interval_ << " minutes\n"
                  << "2. Change interval\n"
                  << "3. Jump to next event\n"
                  << "4. Information\n"
                  << "5. Quit\n"
                  << "Choice: ";

        int choice;
        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice)
        {
        case 1:
            sim_.step(interval_);
            printEventLog();
            sim_.clearEventLog();
            break;
        case 2:
            std::cout << "New interval (minutes): ";
            std::cin >> interval_;
            break;
        case 3:
            sim_.stepToNextEvent();
            printEventLog();
            break;
        case 4:
            showInfoMenu();
            break;
        case 5:
            running = false;
            break;
        default:
            std::cout << "Invalid choice.\n";
        }
    }
}

void UserInterface::showInfoMenu()
{
    bool inMenu = true;
    while (inMenu)
    {
        std::cout << "\n=== INFORMATION ===\n"
                  << "1. Show timetable\n"
                  << "2. Show specific train\n"
                  << "3. Show specific station\n"
                  << "4. Find vehicle by id\n"
                  << "5. Back\n"
                  << "Choice: ";

        int choice;
        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice)
        {
        case 1:
            printTimetable();
            break;
        case 2:
        {
            std::cout << "Train number: ";
            int trainNumber;
            std::cin >> trainNumber;
            showTrainDetails(trainNumber);
            break;
        }
        case 3:
        {
            std::cout << "Station name: ";
            std::string stationName;
            std::cin >> stationName;
            showStationDetails(stationName);
            break;
        }
        case 4:
        {
            std::cout << "Vehicle id: ";
            int vehicleId;
            std::cin >> vehicleId;
            // sim_.findVehicleById(vehicleId);
            break;
        }
        case 5:
            inMenu = false;
            break;
        default:
            std::cout << "Invalid choice.\n";
        }
    }
}

void UserInterface::showStationDetails(const std::string &stationName)
{
    const Station *station = sim_.getStationByName(stationName);
    if (station == nullptr)
    {
        std::cout << "Station not found.\n";
        return;
    }

    std::cout << "\nStation: " << station->getStationName() << "\n\n";

    // Lok
    std::string electric, diesel;
    for (const auto &loco : station->getLocomotives())
    {
        if (loco->getType() == LocomotiveType::Electric)
            electric += std::to_string(loco->getId()) + " ";
        else
            diesel += std::to_string(loco->getId()) + " ";
    }
    std::cout << "Electric locomotives: " << (electric.empty() ? "none" : electric) << "\n";
    std::cout << "Diesel locomotives:   " << (diesel.empty() ? "none" : diesel) << "\n";

    // Vagnar
    std::string seat, sleeping, openFreight, coveredFreight;
    for (const auto &carriage : station->getCarriages())
    {
        switch (carriage->getType())
        {
        case CarriageType::Seat:
            seat += std::to_string(carriage->getId()) + " ";
            break;
        case CarriageType::Sleeping:
            sleeping += std::to_string(carriage->getId()) + " ";
            break;
        case CarriageType::OpenFreight:
            openFreight += std::to_string(carriage->getId()) + " ";
            break;
        case CarriageType::CoveredFreight:
            coveredFreight += std::to_string(carriage->getId()) + " ";
            break;
        }
    }

    std::string trainsAtStation;
    for (auto &train : sim_.getTrains())
    {
        if (train->getDepartureStation() == stationName)
        {
            if (train->getStatus() == TrainStatus::NOT_ASSEMBLED || train->getStatus() == TrainStatus::ASSEMBLED ||
                train->getStatus() == TrainStatus::READY || train->getStatus() == TrainStatus::INCOMPLETE)
            {
                trainsAtStation += std::to_string(train->getTrainNumber()) + " ";
            }
        }
        else if (train->getArrivalStation() == stationName)
        {
            if (train->getStatus() == TrainStatus::ARRIVED)
                trainsAtStation += std::to_string(train->getTrainNumber()) + " ";
        }
    }

    std::cout << "Seat carriages:            " << (seat.empty() ? "none" : seat) << "\n";
    std::cout << "Sleeping carriages:        " << (sleeping.empty() ? "none" : sleeping) << "\n";
    std::cout << "Open freight carriages:    " << (openFreight.empty() ? "none" : openFreight) << "\n";
    std::cout << "Covered freight carriages: " << (coveredFreight.empty() ? "none" : coveredFreight) << "\n";
    std::cout << "Trains:                    " << (trainsAtStation.empty() ? "none" : trainsAtStation) << "\n";
}

std::string statusToString(TrainStatus status)
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

std::string timeToString(int minutes)
{
    int hours = minutes / 60;
    int mins = minutes % 60;
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << hours
        << ":"
        << std::setfill('0') << std::setw(2) << mins;
    return oss.str();
}

std::string vehicleTypeToString(const VehiclePtr &vehicle)
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

void UserInterface::showTrainDetails(int trainNumber)
{
    auto train = sim_.getTrainByNumber(trainNumber);

    if (train == nullptr)
    {
        std::cout << "Train not found.\n";
        return;
    }

    std::cout << "\nDetails about train with train number: " << trainNumber << "\n"
              << "Departure station: " << train->getDepartureStation() << "\n"
              << "Arrival station: " << train->getArrivalStation() << "\n"
              << "Scheduled departure time: " << timeToString(train->getScheduledDepartureTime()) << "\n"
              << "Scheduled arrival time: " << timeToString(train->getScheduledArrivalTime()) << "\n"
              << "Delay: " << train->getDelay() << " minutes\n"
              << "Max speed: " << train->getMaxSpeed() << "\n"
              << "Average speed: " << train->getAverageSpeed() << "\n"
              << "Status: " << statusToString(train->getStatus()) << "\n";
    if (train->getStatus() == TrainStatus::ASSEMBLED ||
        train->getStatus() == TrainStatus::READY ||
        train->getStatus() == TrainStatus::RUNNING ||
        train->getStatus() == TrainStatus::ARRIVED)
    {
        std::cout << "Vehicle sequence:\n";
        for (const auto &vehicle : train->getVehicleSequence())
        {
            std::visit([&vehicle](const auto *v)
                       { std::cout << "  Id: " << v->getId()
                                   << " Type: " << vehicleTypeToString(vehicle) << "\n"; }, vehicle);
        }
    }
}

void printEvent(const SimEvent &event)
{
    switch (event.newStatus)
    {
    case TrainStatus::NOT_ASSEMBLED:
        std::cout << "Train " << event.trainNumber << " has not yet been assembled. \n";
        break;
    case TrainStatus::ASSEMBLED:
        std::cout << "Train " << event.trainNumber << " assembled at station " << event.departureStation << " with sheduled departure time at " << timeToString(event.scheduledDepartureTime) << ". \n";
        break;
    case TrainStatus::READY:
        std::cout << "Train " << event.trainNumber << " is ready for departure at station " << event.departureStation << " at " << timeToString(event.scheduledDepartureTime) << ". \n";
        break;
    case TrainStatus::RUNNING:
        std::cout << "Train " << event.trainNumber << " has left station " << event.departureStation << " at " << timeToString(event.scheduledDepartureTime) << " and are heading for it's final destination, with scheduled arrival time at " << timeToString(event.scheduledArrivalTime) << ".\n ";
        break;
    case TrainStatus::ARRIVED:
        std::cout << "Train " << event.trainNumber << " has arrived at it's final station " << event.arrivalStation << " at " << timeToString(event.scheduledArrivalTime) << ". \n";
        break;
    case TrainStatus::FINISHED:
        std::cout << "Train " << event.trainNumber << " has been disassembled at station. \n";
        break;
    case TrainStatus::INCOMPLETE:
        std::cout << "Unable to assemble train " << event.trainNumber << ". New attempt has been scheduled to " << timeToString(event.scheduledDepartureTime + event.delay) << ". \n";
        break;
    }
}

void UserInterface::printEventLog()
{
    printCurrentTime();
    for (const auto &event : sim_.getEventLog())
    {
        printEvent(event);
    }
}

void UserInterface::printTimetable() const
{

    std::vector<const Train *> sorted;
    for (const auto &train : sim_.getTrains())
        sorted.push_back(train.get());

    std::ranges::sort(sorted, [](const Train *a, const Train *b)
                      { return a->getScheduledDepartureTime() < b->getScheduledDepartureTime(); });

    for (const auto &trains : sorted)
    {
        std::cout << "Train " << trains->getTrainNumber() << " " << trains->getDepartureStation() << " -> " << trains->getArrivalStation() << " Dep: "
                  << timeToString(trains->getScheduledDepartureTime() + trains->getDelay()) << " Arr: " << timeToString(trains->getScheduledArrivalTime() + trains->getDelay())
                  << " Status: " << statusToString(trains->getStatus()) << (trains->getDelay() > 0 ? " (delayed " + std::to_string(trains->getDelay()) + " min)" : "") << ".\n";
    }
}

void UserInterface::printCurrentTime()
{
    int time = sim_.getCurrentTime();
    std::cout << "Current time: " << time / 60 << ":"
              << std::setfill('0') << std::setw(2) << time % 60 << "\n";
}