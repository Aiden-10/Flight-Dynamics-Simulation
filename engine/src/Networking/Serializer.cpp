#include "Serializer.h"

std::string Serializer::toJson(const FlightState& state) {
    std::string j = "{";
    // Position
    j += "\"px\":" + std::to_string(state.px) + ",";
    j += "\"py\":" + std::to_string(state.py) + ",";
    j += "\"pz\":" + std::to_string(state.pz) + ",";
    // Orientation (Quaternion)
    j += "\"qw\":" + std::to_string(state.qw) + ",";
    j += "\"qx\":" + std::to_string(state.qx) + ",";
    j += "\"qy\":" + std::to_string(state.qy) + ",";
    j += "\"qz\":" + std::to_string(state.qz) + ",";
    // Velocity
    j += "\"vx\":" + std::to_string(state.vx) + ",";
    j += "\"vy\":" + std::to_string(state.vy) + ",";
    j += "\"vz\":" + std::to_string(state.vz) + ",";
    // Engine/Environment stats
    j += "\"fuel_mass\":" + std::to_string(state.fuel_mass) + ",";
    j += "\"altitude\":" + std::to_string(state.altitude) + "}";
    return j;
}