// SeatCarriage.h
#ifndef SEATCARRIAGE_H
#define SEATCARRIAGE_H

#include "Carriage.h"
#include "CarriageType.h"

class SeatCarriage : public Carriage
{
private:
    int seats_;
    bool hasInternet_;

public:
    SeatCarriage(int id, int seats, bool hasInternet) : Carriage(id), seats_(seats), hasInternet_(hasInternet) {}
    CarriageType getType() override { return CarriageType::Seat; }
    int getSeats() { return seats_; }
    bool getHasInternet() { return hasInternet_; }
};

#endif