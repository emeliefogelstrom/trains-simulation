#include <gtest/gtest.h>
#include <Station.h>
#include "VehicleTypeUtils.h"
#include "LocomotiveType.h"
#include "CarriageType.h"

TEST(VehicleTypeUtilsTest, IsLocomotive)
{
    EXPECT_TRUE(VehicleTypeUtils::isLocomotive(5));
    EXPECT_FALSE(VehicleTypeUtils::isLocomotive(3));
}

TEST(VehicleTypeUtilsTest, ConvertCarriageType)
{
    EXPECT_EQ(
        VehicleTypeUtils::convertCarriageType(3),
        CarriageType::CoveredFreight);
}

TEST(VehicleTypeUtilsTest, ConvertLocomotiveType)
{
    EXPECT_EQ(
        VehicleTypeUtils::convertLocomotiveType(4),
        LocomotiveType::Electric);
}

TEST(VehicleTypeUtilsTest, InvalidLocomotiveTypeThrows)
{
    EXPECT_THROW(
        VehicleTypeUtils::convertLocomotiveType(6),
        std::invalid_argument);
}

TEST(VehicleTypeUtilsTest, InvalidCarriageTypeThrows)
{
    EXPECT_THROW(
        VehicleTypeUtils::convertCarriageType(10),
        std::invalid_argument);
}