#include <gtest/gtest.h>
#include "FileParser.h"
#include "Track.h"

TEST(FileParserTest, CallFuctionparseTrainMapWithFile)
{
    auto tracks = FileParser::parseTrainMap("../files/TrainMap.txt");
    ASSERT_EQ(tracks.size(), 28);
    EXPECT_EQ(tracks[0].getStationFrom(), "GrandCentral");
    EXPECT_EQ(tracks[0].getStationTo(), "Liege-Guillemins");
    EXPECT_EQ(tracks[0].getDistance(), 190);
}

TEST(FileParserTest, CallFuctionparseTrainsWithFile)
{
    auto tracks = FileParser::parseTrainMap("../files/TrainMap.txt");
    auto trains = FileParser::parseTrains("../files/Trains.txt", tracks);
    ASSERT_EQ(trains.size(), 130);
    EXPECT_EQ(trains[0]->getAverageSpeed(), 126.66666666666667);
    EXPECT_EQ(trains[0]->getDepartureStation(), "GrandCentral");
    EXPECT_EQ(trains[0]->getArrivalStation(), "Liege-Guillemins");
    EXPECT_EQ(trains[0]->getMaxSpeed(), 167);
}

TEST(FileParserTest, CallFunctionparseTrainStationsWithFile)
{
    auto stations = FileParser::parseTrainStations("../files/TrainStations.txt");
    ASSERT_EQ(stations.size(), 8);
    EXPECT_EQ(stations[0]->getStationName(), "GrandCentral");
    EXPECT_GT(stations[0]->getLocomotives().size(), 0);
    EXPECT_GT(stations[0]->getCarriages().size(), 0);
    Locomotive *loco = stations[0]->getLocomotiveById(70);
    ASSERT_NE(loco, nullptr);
    EXPECT_EQ(loco->getId(), 70);
    EXPECT_EQ(loco->getMaxSpeed(), 224);
}