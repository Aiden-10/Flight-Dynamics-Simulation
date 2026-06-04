#pragma once
#include <atomic>
#include <cstdint>

// Command structure for sending commands to the simulation
struct Command {
    // Command types
    enum Type { SPAWN, SET_TARGET, PAUSE };
    Type type;
    // Parameters for the command (e.g., position, velocity, etc.)
    float params[3]; 
};

// Simulation state structure for sending updates to the UI
struct SimulationState {
    // Position
    double px, py, pz;
    // Orientation (Quaternion)
    double qw, qx, qy, qz;
    // Velocity
    double vx, vy, vz;
    // Other stats
    double fuel_mass;
    double altitude;
};

// Metrics structure for tracking network performance
struct NetworkMetrics {
    // std::atomic ensures thread-safe increments without mutexes
    std::atomic<uint64_t> messagesSent{0};
    std::atomic<uint64_t> bytesTransmitted{0};
    std::atomic<uint32_t> failedSends{0};
    std::atomic<uint32_t> loopIterations{0}; 
};
