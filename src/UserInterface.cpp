// UserInterface.cpp

#include "../include/UserInterface.h"
#include "../include/Train.h"
#include "../include/ElectricLocomotive.h"
#include "../include/DieselLocomotive.h"
#include "../include/SeatCarriage.h"
#include "../include/SleepingCarriage.h"
#include "../include/CoveredFreightCarriage.h"
#include "../include/OpenFreightCarriage.h"
#include "../include/SimUtils.h"
#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include <algorithm>

void UserInterface::run()
{
    // Setup menu before simulation starts
    std::cout << "\n=== SIMULATION SETTINGS ===\n"
              << "Start time: " << timeToString(sim_.getStartTime()) << "\n"
              << "End time:   " << timeToString(sim_.getEndTime()) << "\n\n"
              << "1. Change start time\n"
              << "2. Change end time\n"
              << "3. Start simulation\n"
              << "Choice: ";

    int setupChoice;
    while (std::cin >> setupChoice && setupChoice != 3)
    {
        if (setupChoice == 1)
        {
            std::cout << "New start time (hh:mm): ";
            std::string time;
            std::cin >> time;
            sim_.setStartTime(timeFromString(time));
        }
        else if (setupChoice == 2)
        {
            std::cout << "New end time (hh:mm): ";
            std::string time;
            std::cin >> time;
            sim_.setEndTime(timeFromString(time));
        }

        std::cout << "\nStart time: " << timeToString(sim_.getStartTime()) << "\n"
                  << "End time:   " << timeToString(sim_.getEndTime()) << "\n\n"
                  << "1. Change start time\n"
                  << "2. Change end time\n"
                  << "3. Start simulation\n"
                  << "Choice: ";
    }

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
            printCurrentTime();
            printEventLog();
            sim_.clearEventLog();
            if (sim_.isFinished())
            {
                printStatistics();
                running = false;
            }
            break;
        case 2:
            std::cout << "New interval (minutes): ";
            std::cin >> interval_;
            break;
        case 3:
            sim_.stepToNextEvent();
            printCurrentTime();
            printEventLog();
            sim_.clearEventLog();
            if (sim_.isFinished())
            {
                printStatistics();
                running = false;
            }
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
            findVehicleById(vehicleId);
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

void UserInterface::findVehicleById(int vehicleId) const
{
    auto location = sim_.findVehicleById(vehicleId);

    if (!location.found)
    {
        std::cout << "Vehicle " << vehicleId << " not found.\n";
        return;
    }

    if (location.onTrain)
        std::cout << "Vehicle " << vehicleId << " is on train "
                  << location.trainNumber << " ("
                  << statusToString(location.trainStatus) << ").\n";
    else
        std::cout << "Vehicle " << vehicleId << " is at station "
                  << location.stationName << ".\n";
}

void printEvent(const SimEvent &event)
{
    switch (event.newStatus)
    {
    case TrainStatus::NOT_ASSEMBLED:
        std::cout << "Train " << event.trainNumber << " has not yet been assembled. \n";
        break;
    case TrainStatus::ASSEMBLED:
        std::cout << "Train " << event.trainNumber
                  << " assembled at station " << event.departureStation
                  << " with scheduled departure time at "
                  << timeToString(event.scheduledDepartureTime) << ". \n";
        break;
    case TrainStatus::READY:
        std::cout << "Train " << event.trainNumber << " is ready for departure at station " << event.departureStation << " at " << timeToString(event.time + event.delay) << ". \n";
        break;
    case TrainStatus::RUNNING:
        std::cout << "Train " << event.trainNumber << " has left station " << event.departureStation << " at " << timeToString(event.scheduledDepartureTime + event.delay) << " and are heading for it's final destination, with scheduled arrival time at " << timeToString(event.actualArrivalTime) << ".\n";
        break;
    case TrainStatus::ARRIVED:
        std::cout << "Train " << event.trainNumber
                  << " has arrived at it's final station "
                  << event.arrivalStation << " at "
                  << timeToString(event.actualArrivalTime) << ". \n";
        break;
    case TrainStatus::FINISHED:
        std::cout << "Train " << event.trainNumber << " has been disassembled at station. \n";
        break;
    case TrainStatus::INCOMPLETE:
        if (event.time + 10 < event.endTime)
            std::cout << "Unable to assemble train " << event.trainNumber
                      << ". New attempt has been scheduled to "
                      << timeToString(event.time + 10) << ". \n";
        else
            std::cout << "Unable to assemble train " << event.trainNumber
                      << ". No further attempts will be made.\n";
        break;
    }
}

void UserInterface::printEventLog() const
{
    printCurrentTime();
    for (const auto &event : sim_.getEventLog())
    {
        printEvent(event);
    }
}

void UserInterface::printStatistics() const
{
    auto stats = sim_.getTrainStats();

    std::string onTime, delayed, neverDeparted;
    int totalDepartureDelay = 0;
    int totalArrivalDelay = 0;

    for (const auto &s : stats)
    {
        if (!s.departed)
            neverDeparted += std::to_string(s.trainNumber) + " ";
        else if (s.onTime)
            onTime += std::to_string(s.trainNumber) + " ";
        else
        {
            delayed += std::to_string(s.trainNumber) + " ";
            totalDepartureDelay += s.departureDelay;
            totalArrivalDelay += s.arrivalDelay;
        }
    }

    std::cout << "\n=== SIMULATION STATISTICS ===\n"
              << "Trains that arrived on time: " << (onTime.empty() ? "none" : onTime) << "\n"
              << "Trains that never departed: " << (neverDeparted.empty() ? "none" : neverDeparted) << "\n"
              << "Trains that were delayed: " << (delayed.empty() ? "none" : delayed) << "\n\n";

    for (const auto &s : stats)
    {
        if (s.departed && !s.onTime)
            std::cout << "Train " << s.trainNumber
                      << " departed " << s.departureDelay << " min late"
                      << ", arrived " << s.arrivalDelay << " min late\n";
    }

    std::cout << "\nTotal departure delay: " << totalDepartureDelay << " minutes\n";
    std::cout << "Total arrival delay:   " << totalArrivalDelay << " minutes\n";
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

void UserInterface::printCurrentTime() const
{
    int time = sim_.getCurrentTime();
    std::cout << "Current time: " << time / 60 << ":"
              << std::setfill('0') << std::setw(2) << time % 60 << "\n";
}