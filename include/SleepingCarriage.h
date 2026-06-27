// SleepingCarriage.h
#ifndef SLEEPINGCARRIAGE_H
#define SLEEPINGCARRIAGE_H

#include "Carriage.h"
#include "CarriageType.h"

class SleepingCarriage : public Carriage
{
private:
    int beds_;

public:
    SleepingCarriage(int id, int beds) : Carriage(id), beds_(beds) {}
    CarriageType getType() override { return CarriageType::Sleeping; }
    int getBeds() { return beds_; }
};

#endif