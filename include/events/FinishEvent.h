#ifndef FINISH_EVENT_H
#define FINISH_EVENT_H
#include "./Event.h"
#include "../Station.h"
#include "../Train.h"
#include "../VehicleEscrow.h"

class FinishEvent : public Event
{
private:
    Station &station_;
    VehicleEscrow &vehicleEscrow_;

public:
    FinishEvent(Train &train, int time, Station &station, VehicleEscrow &vehicleEscrow) : Event(train, time), station_(station), vehicleEscrow_(vehicleEscrow) {}
    std::unique_ptr<Event> processEvent() override
    {
        train_.finish(station_, vehicleEscrow_);
        return nullptr;
    }
};

#endif