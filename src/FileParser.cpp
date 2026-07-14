// FileParser.cpp
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include "../include/FileParser.h"
#include "../include/SeatCarriage.h"
#include "../include/SleepingCarriage.h"
#include "../include/OpenFreightCarriage.h"
#include "../include/CoveredFreightCarriage.h"
#include "../include/DieselLocomotive.h"
#include "../include/ElectricLocomotive.h"

namespace
{
    void parseVehicle(const std::string &vehicleData, Station &station)
    {
        std::stringstream ss(vehicleData);

        int id;
        int type;

        if (!(ss >> id >> type))
        {
            std::cerr << "Could not parse vehicle header.\n";
            return;
        }

        switch (type)
        {
        case 0:
        {
            int seats, internet;

            if (!(ss >> seats >> internet))
            {
                std::cerr << "Invalid SeatCarriage data.\n";
                return;
            }

            station.addCarriage(
                std::make_unique<SeatCarriage>(id, seats, internet));

            return;
        }

        case 1:
        {
            int beds;

            if (!(ss >> beds))
            {
                std::cerr << "Invalid SleepingCarriage data.\n";
                return;
            }

            station.addCarriage(
                std::make_unique<SleepingCarriage>(id, beds));

            return;
        }

        case 2:
        {
            int capacity, area;

            if (!(ss >> capacity >> area))
            {
                std::cerr << "Invalid OpenFreightCarriage data.\n";
                return;
            }

            station.addCarriage(
                std::make_unique<OpenFreightCarriage>(id, capacity, area));

            return;
        }

        case 3:
        {
            int volume;

            if (!(ss >> volume))
            {
                std::cerr << "Invalid CoveredFreightCarriage data.\n";
                return;
            }

            station.addCarriage(
                std::make_unique<CoveredFreightCarriage>(id, volume));

            return;
        }

        case 4:
        {
            int maxSpeed, consumption;

            if (!(ss >> maxSpeed >> consumption))
            {
                std::cerr << "Invalid ElectricLocomotive data.\n";
                return;
            }

            station.addLocomotive(
                std::make_unique<ElectricLocomotive>(
                    id, maxSpeed, consumption));

            return;
        }

        case 5:
        {
            int maxSpeed, consumption;

            if (!(ss >> maxSpeed >> consumption))
            {
                std::cerr << "Invalid DieselLocomotive data.\n";
                return;
            }

            station.addLocomotive(
                std::make_unique<DieselLocomotive>(
                    id, maxSpeed, consumption));

            return;
        }

        default:
            std::cerr << "Unknown vehicle type: " << type << '\n';
            return;
        }
    }

    std::unique_ptr<Station> parseStation(const std::string &line)
    {
        size_t pos = line.find('(');

        if (pos == std::string::npos)
            return nullptr;

        std::string stationName = line.substr(0, pos);

        if (!stationName.empty() && stationName.back() == ' ')
            stationName.pop_back();

        auto station = std::make_unique<Station>(stationName);

        std::string data = line.substr(pos);

        size_t start = 0;

        while ((start = data.find('(', start)) != std::string::npos)
        {
            size_t end = data.find(')', start);

            if (end == std::string::npos)
                break;

            parseVehicle(
                data.substr(start + 1, end - start - 1),
                *station);

            start = end + 1;
        }

        return station;
    }

    int timeToMinutes(const std::string &time)
    {
        int hours;
        int minutes;
        char colon;

        std::stringstream ss(time);

        if (!(ss >> hours >> colon >> minutes) || colon != ':')
        {
            throw std::runtime_error("Invalid time format: " + time);
        }

        return hours * 60 + minutes;
    }

    int findDistance(
        const std::vector<Track> &tracks,
        const std::string &from,
        const std::string &to)
    {
        for (const auto &track : tracks)
        {
            bool forward =
                track.getStationFrom() == from &&
                track.getStationTo() == to;

            bool reverse =
                track.getStationFrom() == to &&
                track.getStationTo() == from;

            if (forward || reverse)
                return track.getDistance();
        }

        throw std::runtime_error(
            "Track not found: " + from + " -> " + to);
    }
}

std::vector<std::unique_ptr<Station>> FileParser::parseTrainStations(const std::string &filename)
{
    std::vector<std::unique_ptr<Station>> stations;

    std::ifstream file(filename);

    if (!file)
        return stations;

    std::string line;

    while (std::getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        if (auto station = parseStation(line))
            stations.push_back(std::move(station));
    }

    return stations;
}

std::vector<Track> FileParser::parseTrainMap(const std::string &filename)
{
    std::vector<Track> tracks;

    std::ifstream file(filename);

    if (!file)
        return tracks;

    std::string line;

    while (std::getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        std::stringstream ss(line);

        std::string stationFrom;
        std::string stationTo;
        int distance;

        if (!(ss >> stationFrom >> stationTo >> distance))
        {
            std::cerr << "Invalid track data: " << line << '\n';
            continue;
        }

        tracks.emplace_back(stationFrom, stationTo, distance);
    }

    return tracks;
}

std::vector<std::unique_ptr<Train>> FileParser::parseTrains(const std::string &filename, const std::vector<Track> &tracks)
{
    std::vector<std::unique_ptr<Train>> trains;
    std::ifstream file(filename);

    if (!file)
        return trains;

    std::string line;

    while (std::getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        std::stringstream ss(line);

        std::string fromStation, toStation, departureTime, arrivalTime;
        int trainId, maxSpeed, vehicleType;
        std::vector<int> requiredTypes;

        if (!(ss >> trainId >> fromStation >> toStation >> departureTime >> arrivalTime >> maxSpeed))
        {
            std::cerr << "Invalid train data: " << line << '\n';
            continue;
        }

        while (ss >> vehicleType)
        {
            requiredTypes.push_back(vehicleType);
        }

        int departureMinutes = timeToMinutes(departureTime);
        int arrivalMinutes = timeToMinutes(arrivalTime);
        int distance = findDistance(tracks, fromStation, toStation);

        trains.emplace_back(std::make_unique<Train>(trainId, fromStation, toStation, departureMinutes, arrivalMinutes, maxSpeed, distance, requiredTypes));
    }

    return trains;
}