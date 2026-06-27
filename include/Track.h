// Track.h
#pragma once
#include <string>

class Track
{
private:
    std::string stationFrom_;
    std::string stationTo_;
    int distance_;

public:
    Track(std::string stationFrom, std::string stationTo, int distance) : stationFrom_(stationFrom), stationTo_(stationTo), distance_(distance) {}
    const std::string &getStationFrom() const { return stationFrom_; }
    const std::string &getStationTo() const { return stationTo_; }
    int getDistance() const { return distance_; }
};