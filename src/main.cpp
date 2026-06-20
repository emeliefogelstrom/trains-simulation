#include <iostream>
#include "ElectricLocomotive.h"
#include "DieselLocomotive.h"
#include "SeatCarriage.h"
#include "SleepingCarriage.h"
#include "OpenFreightCarriage.h"
#include "CoveredFreightCarriage.h"

int main()
{
    ElectricLocomotive elLoco{70, 224, 4955};
    std::cout << "ElectricLocomotive id=" << elLoco.getId()
              << " maxSpeed=" << elLoco.getMaxSpeed()
              << " consumption=" << elLoco.getConsumption() << "\n";

    DieselLocomotive dieselLoco{71, 180, 350};
    std::cout << "DieselLocomotive id=" << dieselLoco.getId()
              << " maxSpeed=" << dieselLoco.getMaxSpeed()
              << " consumption=" << dieselLoco.getConsumption() << "\n";

    SeatCarriage seat{1, 100, true};
    std::cout << "SeatCarriage id=" << seat.getId()
              << " seats=" << seat.getSeats() << "\n";

    SleepingCarriage sleeping{2, 30};
    std::cout << "SleepingCarriage id=" << sleeping.getId()
              << " beds=" << sleeping.getBeds() << "\n";

    OpenFreightCarriage openFreight{3, 50, 20};
    std::cout << "OpenFreightCarriage id=" << openFreight.getId() << "\n";

    CoveredFreightCarriage coveredFreight{4, 97};
    std::cout << "CoveredFreightCarriage id=" << coveredFreight.getId() << "\n";

    return 0;
}