// DieselLocomotive.h
#pragma once

#include "Locomotive.h"
#include "LocomotiveType.h"

class DieselLocomotive : public Locomotive
{

private:
    int consumption_;

public:
    DieselLocomotive(int id, int maxSpeed, int consumption) : Locomotive(id, maxSpeed), consumption_(consumption) {}
    LocomotiveType getType() override { return LocomotiveType::Diesel; }
    int getConsumption() override { return consumption_; }
};