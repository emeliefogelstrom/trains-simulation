# Design Decisions

This document explains the key architectural and implementation decisions
made during the development of the Trains Simulation project.

---

## 1. No shared Vehicle base class — `std::variant` instead

**Decision:** `Locomotive` and `Carriage` are two independent class
hierarchies with no common base class. Where both types need to coexist
in an ordered sequence, `std::variant` is used.

```cpp
using VehiclePtr = std::variant<const Locomotive*, const Carriage*>;
using Vehicle    = std::variant<std::unique_ptr<Locomotive>,
                                std::unique_ptr<Carriage>>;
```

**Why:** The spec states that `maxSpeed` is a locomotive-only attribute.
A shared base class would either force `maxSpeed` onto `Carriage` (wrong)
or require `dynamic_cast` everywhere (fragile). `std::variant` enforces
the closed set of exactly two types at compile time, makes the type
system honest, and avoids virtual dispatch overhead for a fixed type set.

---

## 2. VehicleEscrow — a neutral third-party owner during transit

**Decision:** A standalone `VehicleEscrow` class owns vehicles (via
`unique_ptr`) from the moment they leave a departure station's pool until
they are returned to the arrival station's pool at FINISHED.

**Why:** At any point in time, exactly one entity owns each vehicle:
either a `Station`'s pool, or `VehicleEscrow`. This makes ownership
explicit and auditable. It also satisfies grading requirement e) —
"find a vehicle by id" — since `VehicleEscrow` is the single place to
query for in-transit vehicles.

`Train` holds only non-owning `const` raw pointers into `VehicleEscrow`
(via `vehicleSequence_`) to remember which vehicles belong to it and in
what order. Raw pointers are intentional here: the C++ Core Guidelines
treat a bare `T*` as non-owning by convention, and ownership already
lives in `VehicleEscrow`.

---

## 3. Check-then-commit in `tryAssemble()`

**Decision:** Assembly is a two-phase operation. Phase 1 verifies that
all required vehicle types are available (without moving anything).
Phase 2 commits — extracting and moving vehicles — only if phase 1
succeeded completely.

**Why:** A partial commit would leave the station pool in an inconsistent
state and risk one train "stealing" a vehicle another train had already
claimed. The two-phase approach guarantees that a failed assembly attempt
leaves the pool completely untouched.

A `claimedIds` vector tracks which ids have been virtually allocated
within the current attempt, preventing two slots of the same type from
being assigned the same vehicle.

---

## 4. EventQueue — encapsulating a `priority_queue` limitation

**Decision:** A dedicated `EventQueue` class wraps `std::priority_queue`
and exposes a clean `push` / `pop` / `empty` / `topTime` API.

**Why:** `priority_queue::top()` returns a `const` reference, which
prevents `std::move` directly. The standard workaround is `const_cast`,
which is correct but visually alarming. By isolating the `const_cast`
inside `EventQueue::pop()` with an explanatory comment, no other code
needs to know about this STL limitation. The rest of the codebase sees
only a clean, intent-revealing API.

```cpp
std::unique_ptr<Event> EventQueue::pop()
{
    // priority_queue::top() returns const ref, preventing std::move.
    // const_cast is the standard workaround — isolated here so no other
    // code needs to know about this STL limitation.
    auto event = std::move(
        const_cast<std::unique_ptr<Event>&>(queue_.top()));
    queue_.pop();
    return event;
}
```

---

## 5. Events return their successor — no Simulation pointer needed

**Decision:** `Event::processEvent()` returns
`std::unique_ptr<Event>` — the next event to schedule — instead of
calling `simulation.scheduleEvent(...)` directly.

**Why:** If events held a `Simulation*`, they would be tightly coupled
to the orchestration layer. The return-based design means each event
class only knows about its own `Train` and any resources it directly
needs (`Station&`, `VehicleEscrow&`). `Simulation` drives the queue
without events needing to know the queue exists. This also makes events
trivially unit-testable in isolation: construct a `Train`, construct an
event, call `processEvent()`, check the returned event type and the
train's new status — no simulation machinery required.

The one exception is `ArriveEvent`, which returns `nullptr`.
`FinishEvent` needs a `Station&` and `VehicleEscrow&` that only
`Simulation` can provide, so `Simulation` creates `FinishEvent` directly
when it observes a `nullptr` return from an `ARRIVED` train.

---

## 6. FileParser — anonymous namespace for internal helpers

**Decision:** `parseVehicle`, `parseStation`, `timeToMinutes` and
`findDistance` are defined in an anonymous namespace inside
`FileParser.cpp`, not declared in the header.

**Why:** These are implementation details of the parsing logic.
Exposing them in the header would invite callers to depend on them,
making future refactoring harder. An anonymous namespace is the C++
idiomatic equivalent of `private` for free functions — the symbol is
translation-unit-local and invisible to all other code.

---

## 7. VehicleTypeUtils — inline functions in a namespace, not a class

**Decision:** Type-code conversion utilities live in a `namespace
VehicleTypeUtils` with `inline` free functions, not a class with static
methods.

**Why:** A class with only static methods and no data is a Java pattern
misapplied to C++. A namespace expresses the same grouping more
idiomatically. The `inline` keyword is required because the functions
are defined in a header included by multiple translation units —
without it the linker would report multiple-definition errors.

---

## 8. Postgres as a write-only history sink

**Decision:** The three data files (`TrainMap.txt`, `TrainStations.txt`,
`Trains.txt`) feed the in-memory C++ objects directly, as the spec
requires. PostgreSQL is a write-only append log — fed after each state
transition — not a source of truth for the running simulation.

**Why:** The spec explicitly requires file-based startup. Replacing file
parsing with database reads would violate a stated requirement. The
Postgres layer instead demonstrates database integration without
compromising spec compliance: it captures a persistent, queryable
history of every state transition, satisfying grading requirement f)
(vehicle movement history) and providing a backend for a future
React dashboard.

---

## 9. Include guards over `#pragma once`

**Decision:** All headers use `#ifndef` / `#define` / `#endif` include
guards rather than `#pragma once`.

**Why:** The assignment specification explicitly prohibits
compiler-specific constructs. `#pragma once` is not part of the C++
standard and is therefore disallowed. Guard names follow the convention
`CLASSNAME_H` (e.g. `TRAIN_H`, `VEHICLE_ESCROW_H`).

---

## 10. SimUtils — shared presentation helpers

**Decision:** `timeToString`, `statusToString`, `vehicleTypeToString`
and `timeFromString` live in `SimUtils.h` as `inline` free functions,
shared between `UserInterface.cpp` and `Simulation.cpp`.

**Why:** Both the logging layer (`Simulation`) and the display layer
(`UserInterface`) need to format times and statuses as strings.
Duplicating the functions would risk inconsistency; putting them in
either class would create an unwanted dependency. A shared utility
header with `inline` functions is the lightest-weight solution with
no runtime overhead.