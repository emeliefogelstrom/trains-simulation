#include <gtest/gtest.h>
#include "../include/VehicleEscrow.h"
#include "../include/DieselLocomotive.h"

TEST(VehicleEscrowTest, StoreThenExtractReturnsCorrectVehicle)
{
    VehicleEscrow escrow;
    auto loco = std::make_unique<DieselLocomotive>(7, 140, 431);
    escrow.add(std::move(loco));

    Vehicle result = escrow.extract(7);

    ASSERT_TRUE(std::holds_alternative<std::unique_ptr<Locomotive>>(result));
    auto &extracted = std::get<std::unique_ptr<Locomotive>>(result);
    ASSERT_NE(extracted, nullptr);
    ASSERT_EQ(extracted->getId(), 7);
}

TEST(VehicleEscrowTest, ExtractRemovesVehicleFromStorage)
{
    VehicleEscrow escrow;
    auto loco = std::make_unique<DieselLocomotive>(7, 140, 431);
    escrow.add(std::move(loco));

    escrow.extract(7);

    ASSERT_EQ(escrow.getStorageSize(), 0);
}

TEST(VehicleEscrowTest, ExtractThrowsOnMissingId)
{
    VehicleEscrow escrow;
    EXPECT_THROW(escrow.extract(999), std::runtime_error);
}