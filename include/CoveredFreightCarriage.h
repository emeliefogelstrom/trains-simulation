// CoveredFreightCarriage.h
#pragma once
#include "Carriage.h"
#include "CarriageType.h"

class CoveredFreightCarriage : public Carriage
{
private:
    int volumeCubicMeters_;

public:
    CoveredFreightCarriage(int id, int volumeCubicMeters) : Carriage(id), volumeCubicMeters_(volumeCubicMeters) {}
    CarriageType getType() override { return CarriageType::CoveredFreight; }
    int getVolumeCubicMeters() { return volumeCubicMeters_; }
};