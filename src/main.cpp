#include <iostream>
#include "ElectricLocomotive.h"
#include "DieselLocomotive.h"
#include "SeatCarriage.h"
#include "SleepingCarriage.h"
#include "OpenFreightCarriage.h"
#include "CoveredFreightCarriage.h"
#include "Station.h"
#include "UserInterface.h"
#include <FileParser.h>

int main()
{
    std::cout << "========================================\n"
              << "   Ironbend Train and Brain Railway\n"
              << "         Train Simulation v1.0\n"
              << "========================================\n\n"
              << "Loading simulation data...\n";

    auto tracks = FileParser::parseTrainMap("../files/TrainMap.txt");
    auto stations = FileParser::parseTrainStations("../files/TrainStations.txt");
    auto trains = FileParser::parseTrains("../files/Trains.txt", tracks);

    std::cout << "Loaded " << trains.size() << " trains, "
              << stations.size() << " stations, "
              << tracks.size() << " tracks.\n\n";

    Simulation sim(std::move(trains), std::move(stations), tracks);
    UserInterface ui(sim);
    ui.run();

    return 0;
}