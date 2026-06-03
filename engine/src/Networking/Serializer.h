#pragma once
#include <string>
#include "DataStructures.h"

class Serializer {
public:
    static std::string toJson(const FlightState& state);
};