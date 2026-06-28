#include <gtest/gtest.h>
#include "Track.h"

TEST(TrackTest, StoresStationsAndDistance)
{
    Track track("StationA", "StationB", 120);
    EXPECT_EQ(track.getStationFrom(), "StationA");
    EXPECT_EQ(track.getStationTo(), "StationB");
    EXPECT_EQ(track.getDistance(), 120);
}