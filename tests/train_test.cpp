#include <gtest/gtest.h>
#include <Station.h>
#include "Train.h"
#include <DieselLocomotive.h>
#include <ElectricLocomotive.h>
#include <CoveredFreightCarriage.h>
#include <SeatCarriage.h>

TEST(TrainTest, AssemblesTrainAndStatusIsAssembled)
{
    Station departure("Grand Central");
    departure.addLocomotive(std::make_unique<DieselLocomotive>(1, 160, 300));
    departure.addCarriage(std::make_unique<CoveredFreightCarriage>(2, 50));
    departure.addCarriage(std::make_unique<CoveredFreightCarriage>(3, 50));
    departure.addCarriage(std::make_unique<CoveredFreightCarriage>(4, 50));

    VehicleEscrow box;
    Train train(123, "Grand Central", "St. Pancras", 765, 965, 145, 165, {5, 3, 3, 3});

    EXPECT_TRUE(train.tryAssemble(departure, box));
    EXPECT_EQ(train.getStatus(), TrainStatus::ASSEMBLED);
    EXPECT_EQ(departure.getCarriages().size(), 0);
    EXPECT_EQ(departure.getLocomotives().size(), 0);
    EXPECT_EQ(box.getStorageSize(), 4);
}

TEST(TrainTest, AssemblesTrainWithMissingCarriageStatusIsIncomplete)
{
    Station departure("Grand Central");
    departure.addLocomotive(std::make_unique<DieselLocomotive>(1, 160, 300));
    departure.addCarriage(std::make_unique<SeatCarriage>(2, 125, 0));
    departure.addCarriage(std::make_unique<CoveredFreightCarriage>(3, 50));
    departure.addCarriage(std::make_unique<CoveredFreightCarriage>(4, 50));

    VehicleEscrow box;
    Train train(123, "Grand Central", "St. Pancras", 765, 965, 145, 165, {5, 0, 1});

    EXPECT_FALSE(train.tryAssemble(departure, box));
    EXPECT_EQ(train.getStatus(), TrainStatus::INCOMPLETE);
    EXPECT_EQ(departure.getCarriages().size(), 3);
    EXPECT_EQ(departure.getLocomotives().size(), 1);
    EXPECT_EQ(box.getStorageSize(), 0);
}

TEST(TrainTest, AssemblesTrainWithOnlyOneElectricLocomotiveAvailableStatusIsIncomplete)
{
    Station departure("Grand Central");
    departure.addLocomotive(std::make_unique<ElectricLocomotive>(1, 125, 1542));
    departure.addCarriage(std::make_unique<SeatCarriage>(2, 125, 0));
    departure.addCarriage(std::make_unique<CoveredFreightCarriage>(3, 50));
    departure.addCarriage(std::make_unique<CoveredFreightCarriage>(4, 50));

    VehicleEscrow box;
    Train train(123, "Grand Central", "St. Pancras", 765, 965, 145, 165, {4, 0, 3, 3, 4});

    EXPECT_FALSE(train.tryAssemble(departure, box));
    EXPECT_EQ(train.getStatus(), TrainStatus::INCOMPLETE);
    EXPECT_EQ(departure.getCarriages().size(), 3);
    EXPECT_EQ(departure.getLocomotives().size(), 1);
    EXPECT_EQ(box.getStorageSize(), 0);
}

TEST(TrainTest, DisassembleTrainAndMoveVehiclesToArrivalStation)
{
    Station departure("Grand Central");
    departure.addLocomotive(std::make_unique<ElectricLocomotive>(1, 125, 1542));
    departure.addCarriage(std::make_unique<SeatCarriage>(2, 125, 0));
    departure.addCarriage(std::make_unique<CoveredFreightCarriage>(3, 50));
    departure.addCarriage(std::make_unique<CoveredFreightCarriage>(4, 50));
    departure.addLocomotive(std::make_unique<ElectricLocomotive>(5, 123, 1462));

    VehicleEscrow box;
    Train train(123, "Grand Central", "St. Pancras", 765, 965, 145, 165, {4, 0, 3, 3, 4});
    train.tryAssemble(departure, box);

    Station arrival("St. Pancras");
    train.finish(arrival, box);

    EXPECT_EQ(arrival.getCarriages().size(), 3);
    EXPECT_EQ(arrival.getLocomotives().size(), 2);
    EXPECT_EQ(box.getStorageSize(), 0);
    EXPECT_EQ(train.getStatus(), TrainStatus::FINISHED);
}