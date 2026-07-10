#include <gtest/gtest.h>
#include <Station.h>
#include <vector>
#include "DieselLocomotive.h"
#include "SleepingCarriage.h"
#include "LocomotiveType.h"
#include "CarriageType.h"
#include "VehicleTypeUtils.h"

TEST(StationTest, extractLocomotiveById)
{
    Station station("Grand Central");
    station.addLocomotive(std::make_unique<DieselLocomotive>(1, 145, 400));
    auto locomotive = station.extractLocomotiveById(1);

    ASSERT_NE(locomotive, nullptr);
    EXPECT_EQ(locomotive->getId(), 1);
    EXPECT_EQ(station.getLocomotives().size(), 0);
}

TEST(StationTest, extractCarriageById)
{
    Station station("Grand Central");
    station.addCarriage(std::make_unique<SleepingCarriage>(6, 166));
    auto carriage = station.extractCarriageById(6);

    ASSERT_NE(carriage, nullptr);
    EXPECT_EQ(carriage->getId(), 6);
    EXPECT_EQ(station.getCarriages().size(), 0);
}

TEST(StationTest, getLowestIdByLocomotiveType)
{
    Station station("Grand Central");
    station.addLocomotive(std::make_unique<DieselLocomotive>(1, 145, 400));
    station.addLocomotive(std::make_unique<DieselLocomotive>(7, 187, 300));

    auto result = station.getLowestIdByLocomotiveType(
        VehicleTypeUtils::convertLocomotiveType(5), {1});
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, 7);
}

TEST(StationTest, getLowestIdByCarriageType)
{
    Station station("Grand Central");
    station.addCarriage(std::make_unique<SleepingCarriage>(1, 125));
    station.addCarriage(std::make_unique<SleepingCarriage>(7, 190));

    auto result = station.getLowestIdByCarriageType(
        VehicleTypeUtils::convertCarriageType(1), {1});
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, 7);
}

TEST(StationTest, emptyExtractLocomotiveById)
{
    Station station("Grand Central");

    EXPECT_EQ(station.extractLocomotiveById(1), nullptr);
}

TEST(StationTest, emptyExtractCarriageById)
{
    Station station("Grand Central");

    EXPECT_EQ(station.extractCarriageById(1), nullptr);
}

TEST(StationTest, getLowestIdByLocomotiveTypeReturnsEmptyWhenNotFound)
{
    Station station("Grand Central");
    auto result = station.getLowestIdByLocomotiveType(
        LocomotiveType::Electric, {});
    EXPECT_FALSE(result.has_value());
}

TEST(StationTest, getLowestIdByCarriageTypeReturnsEmptyWhenNotFound)
{
    Station station("Grand Central");
    auto result = station.getLowestIdByCarriageType(
        CarriageType::Sleeping, {});
    EXPECT_FALSE(result.has_value());
}