#ifndef READY_EVENT_H
#define READY_EVENT_H
#include "./Event.h"
#include "./DepartEvent.h"

class ReadyEvent : public Event
{
public:
    ReadyEvent(Train *train, int time) : Event(train, time) {}
    std::unique_ptr<Event> processEvent() override
    {
        train_->markReady();
        return std::make_unique<DepartEvent>(train_, train_->getScheduledDepartureTime());
    }
};

#endif