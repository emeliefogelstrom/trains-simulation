// Carriage.h
#pragma once
#include "CarriageType.h"

class Carriage
{
protected:
    int id_;

public:
    Carriage(int id) : id_(id) {}
    virtual ~Carriage() = default;
    virtual CarriageType getType() = 0;
    int getId() const { return id_; }
};