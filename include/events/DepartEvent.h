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
        train_.depart();

        int arrivalTime = train_.getScheduledArrivalTime();

        if (train_.getDelay() > 0)
        {
            double timeAtMaxSpeed = static_cast<double>(train_.getDistance()) / train_.getMaxSpeed() * 60;
            int earliestArrival = train_.getScheduledDepartureTime() + train_.getDelay() + static_cast<int>(timeAtMaxSpeed);

            if (earliestArrival > train_.getScheduledArrivalTime())
                arrivalTime = earliestArrival;
        }

        train_.setActualArrivalTime(arrivalTime);
        return std::make_unique<ArriveEvent>(train_, arrivalTime);
    }
};

#endif