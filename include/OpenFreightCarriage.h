// OpenFreightCarriage.h
#pragma once
#include "Carriage.h"
#include "CarriageType.h"

class OpenFreightCarriage : public Carriage
{
private:
    int loadCapacityTon_, loadAreaSquareMeters_;

public:
    OpenFreightCarriage(int id, int loadCapacityTon, int loadAreaSquareMeters) : Carriage(id), loadCapacityTon_(loadCapacityTon), loadAreaSquareMeters_(loadAreaSquareMeters) {}
    CarriageType getType() override { return CarriageType::OpenFreight; }
    int getLoadCapacityTon() { return loadCapacityTon_; }
    int getLoadAreaSquareMeters() { return loadAreaSquareMeters_; }
};