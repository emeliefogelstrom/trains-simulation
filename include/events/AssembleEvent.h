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

public:
    AssembleEvent(Train &train, int time, Station &station, VehicleEscrow &vehicleEscrow) : Event(train, time), station_(station), vehicleEscrow_(vehicleEscrow) {}

    std::unique_ptr<Event> processEvent() override
    {
        if (train_.tryAssemble(station_, vehicleEscrow_))
        {
            return std::make_unique<ReadyEvent>(train_, train_.getScheduledDepartureTime() - 10);
        }

        train_.addDelay(10);

        return std::make_unique<AssembleEvent>(
            train_,
            time_ + 10,
            station_,
            vehicleEscrow_);
    }
};

#endif