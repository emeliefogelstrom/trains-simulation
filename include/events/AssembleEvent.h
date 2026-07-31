#ifndef ASSEMBLE_EVENT_H
#define ASSEMBLE_EVENT_H
#include "./Event.h"
#include "../Station.h"
#include "../VehicleEscrow.h"
#include "./ReadyEvent.h"

class AssembleEvent : public Event
{
private:
    Station &station_;
    VehicleEscrow &vehicleEscrow_;
    int endTime_;

public:
    AssembleEvent(Train &train, int time, Station &station, VehicleEscrow &vehicleEscrow, int endTime)
        : Event(train, time), station_(station), vehicleEscrow_(vehicleEscrow), endTime_(endTime) {}

    std::unique_ptr<Event> processEvent() override
    {
        if (time_ >= endTime_)
            return nullptr;

        if (train_.tryAssemble(station_, vehicleEscrow_))
        {
            return std::make_unique<ReadyEvent>(
                train_,
                train_.getScheduledDepartureTime() + train_.getDelay() - 10);
        }

        train_.addDelay(10);

        return std::make_unique<AssembleEvent>(
            train_,
            time_ + 10,
            station_,
            vehicleEscrow_,
            endTime_);
    }
};

#endif