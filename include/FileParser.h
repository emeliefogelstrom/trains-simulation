#ifndef FILEPARSER_H
#define FILEPARSER_H
#include <vector>
#include <memory>
#include <string>
#include "Track.h"
#include "Station.h"
#include "Train.h"

namespace FileParser
{
    std::vector<Track> parseTrainMap(const std::string &filename);
    std::vector<std::unique_ptr<Station>> parseTrainStations(const std::string &filename);
    std::vector<std::unique_ptr<Train>> parseTrains(const std::string &filename, const std::vector<Track> &tracks);
}

#endif