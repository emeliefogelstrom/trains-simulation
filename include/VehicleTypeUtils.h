#ifndef VEHICLETYPEUTILS_H
#define VEHICLETYPEUTILS_H
#include "CarriageType.h"
#include "LocomotiveType.h"
#include <stdexcept>
#include <string>

namespace VehicleTypeUtils
{
    inline bool isLocomotive(int type)
    {
        return type == 4 || type == 5;
    }

    inline CarriageType convertCarriageType(int typeNumber)
    {
        switch (typeNumber)
        {
        case 0:
            return CarriageType::Seat;
        case 1:
            return CarriageType::Sleeping;
        case 2:
            return CarriageType::OpenFreight;
        case 3:
            return CarriageType::CoveredFreight;
        default:
            throw std::invalid_argument("Unknown carriage type: " + std::to_string(typeNumber));
        }
    }

    inline LocomotiveType convertLocomotiveType(int typeNumber)
    {

        switch (typeNumber)
        {
        case 4:
            return LocomotiveType::Electric;
        case 5:
            return LocomotiveType::Diesel;
        default:
            throw std::invalid_argument("Unknown locomotive type: " + std::to_string(typeNumber));
        }
    }
};

#endif