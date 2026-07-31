// EventQueue.h
#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H
#include <memory>
#include <queue>
#include "Event.h"

struct EventComparator
{
    bool operator()(const std::unique_ptr<Event> &a,
                    const std::unique_ptr<Event> &b) const
    {
        if (a->getTime() != b->getTime())
            return a->getTime() > b->getTime();
        return a->getTrainNumber() > b->getTrainNumber();
    }
};

class EventQueue
{
private:
    std::priority_queue<
        std::unique_ptr<Event>,
        std::vector<std::unique_ptr<Event>>,
        EventComparator>
        queue_;

public:
    void pushEvent(std::unique_ptr<Event> event)
    {
        queue_.push(std::move(event));
    }
    std::unique_ptr<Event> pop()
    {
        // priority_queue::top() returns const ref, preventing std::move directly.
        // const_cast is the standard workaround — isolated here so no other
        // code needs to know about this STL limitation.
        auto event = std::move(const_cast<std::unique_ptr<Event> &>(queue_.top()));
        queue_.pop();
        return event;
    }
    bool empty() const { return queue_.empty(); }
    int topTime() const { return queue_.top()->getTime(); }
    void removeIfInactive()
    {
        std::vector<std::unique_ptr<Event>> keep;
        while (!queue_.empty())
        {
            auto event = std::move(const_cast<std::unique_ptr<Event> &>(queue_.top()));
            queue_.pop();
            TrainStatus s = event->getTrain().getStatus();
            if (s == TrainStatus::RUNNING ||
                s == TrainStatus::ARRIVED)
            {
                keep.push_back(std::move(event));
            }
        }
        for (auto &e : keep)
            queue_.push(std::move(e));
    }
};

#endif