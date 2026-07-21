#ifndef DEPART_EVENT_H
#define DEPART_EVENT_H
#include "./Event.h"
#include "./ArriveEvent.h"

class DepartEvent : public Event
{
public:
    DepartEvent(Train &train, int time) : Event(train, time) {}
    std::unique_ptr<Event> processEvent() override
    {
        std::cout << "I DepartEvent för tåg: " << train_.getTrainNumber() << std::endl;
        train_.depart();
        return std::make_unique<ArriveEvent>(train_, train_.getScheduledArrivalTime() + train_.getDelay());
    }
};

#endif