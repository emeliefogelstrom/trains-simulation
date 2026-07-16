#ifndef EVENT_H
#define EVENT_H
#include <iostream>
#include "../Train.h"

class Event
{
protected:
    Train *train_;
    int time_;

public:
    Event(Train *train, int time) : train_(train), time_(time) {}
    virtual std::unique_ptr<Event> processEvent() = 0;
    virtual ~Event() = default;
    int getTime() const { return time_; }
};

#endif