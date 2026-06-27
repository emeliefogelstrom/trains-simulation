// Carriage.h
#ifndef CARRIAGE_H
#define CARRIAGE_H
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

#endif