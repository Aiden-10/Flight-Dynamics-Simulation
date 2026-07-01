#include "Serializer.h"

/* Example JSON output for a SimulationState object:
{
    "px":10.500000,
    "py":2.000000,
    "pz":100.250000,

    "qw":1.000000,
    "qx":0.000000,
    "qy":0.000000,
    "qz":0.000000,

    "vx":150.200000,
    "vy":-3.100000,
    "vz":12.800000,

    "fuel_mass":84.600000,
    "altitude":100.250000
}
*/

std::string Serializer::toJson(SimulationState state) {
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
    j += "\"mass\":" + std::to_string(state.mass) + ",";
    j += "\"altitude\":" + std::to_string(state.altitude) + "}";
    return j;
}