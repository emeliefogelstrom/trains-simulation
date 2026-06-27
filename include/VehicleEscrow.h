// VehicleEscrow.h
#ifndef VEHICLEESCROW_H
#define VEHICLEESCROW_H

#include <memory>
#include <variant>
#include <vector>
#include <algorithm>
#include <ranges>
#include <stdexcept>
#include "Locomotive.h"
#include "Carriage.h"

using Vehicle =
    std::variant<std::unique_ptr<Locomotive>,
                 std::unique_ptr<Carriage>>;

class VehicleEscrow
{
private:
    std::vector<Vehicle> store_;

public:
    void setStorage(Vehicle store)
    {
        store_.push_back(std::move(store));
    }
    Vehicle extract(int id)
    {
        auto matchesId = [id](const Vehicle &v)
        {
            return std::visit([id](const auto &ptr)
                              { return ptr && ptr->getId() == id; }, v);
        };

        auto it = std::ranges::find_if(store_, matchesId);

        if (it == store_.end())
            throw std::runtime_error("Vehicle not found");

        Vehicle result = std::move(*it);
        store_.erase(it);
        return result;
    }
};

#endif