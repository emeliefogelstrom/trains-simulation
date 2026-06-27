// CoveredFreightCarriage.h
#ifndef COVEREDFREIGHTCARRIAGE_H
#define COVEREDFREIGHTCARRIAGE_H

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

#endif