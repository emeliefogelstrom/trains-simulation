#ifndef DEPART_EVENT_H
#define DEPART_EVENT_H
#include "./Event.h"
#include "./ArriveEvent.h"

class DepartEvent : public Event
{
public:
    DepartEvent(Train *train, int time) : Event(train, time) {}
    std::unique_ptr<Event> processEvent() override
    {
        train_->depart();
        return std::make_unique<ArriveEvent>(train_, train_->getScheduledArrivalTime());
    }
};

#endif