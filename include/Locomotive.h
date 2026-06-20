// Locomotive.h
#pragma once
#include "LocomotiveType.h"

class Locomotive
{
protected:
    int id_, maxSpeed_;

public:
    Locomotive(int id, int maxSpeed) : id_(id), maxSpeed_(maxSpeed) {}
    virtual ~Locomotive() = default;
    virtual LocomotiveType getType() = 0;
    virtual int getConsumption() = 0;
    int getId() const { return id_; }
    int getMaxSpeed() const { return maxSpeed_; }
};