// Track.h
#ifndef TRACK_H
#define TRACK_H

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

#endif