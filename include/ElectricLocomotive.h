// ElectricLocomotive.h
#pragma once
#include "Locomotive.h"
#include "LocomotiveType.h"

class ElectricLocomotive : public Locomotive
{
private:
    int consumption_;

public:
    ElectricLocomotive(int id, int maxSpeed, int consumption) : Locomotive(id, maxSpeed), consumption_(consumption) {}
    LocomotiveType getType() override { return LocomotiveType::Electric; }
    int getConsumption() override { return consumption_; }
};