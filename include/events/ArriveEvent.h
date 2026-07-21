#ifndef ARRIVE_EVENT_H
#define ARRIVE_EVENT_H
#include "./Event.h"
#include "./FinishEvent.h"
#include "../Station.h"
#include "../VehicleEscrow.h"

class ArriveEvent : public Event
{
public:
    ArriveEvent(Train &train, int time) : Event(train, time) {}
    std::unique_ptr<Event> processEvent() override
    {
        train_.arrive();
        return nullptr;
    }
};

#endif