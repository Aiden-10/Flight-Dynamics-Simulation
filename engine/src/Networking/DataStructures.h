#pragma once
#include <atomic>
#include <cstdint>
#include <vector>

// Command structure for sending commands to the simulation
struct CommandPacket {
    // Command types
    enum Type { RUN, HOLD, RESET, SETTINGS, EXIT };
    Type type;
    // JSON string containing command parameters
    std::string parameters;
};

// Simulation state structure for sending updates to the UI
struct SimulationState {
    double currentTime;
    // Position
    double px, py, pz;
    // Orientation (Quaternion)
    double qw, qx, qy, qz;
    // Velocity
    double vx, vy, vz;
    // Other stats
    double mass;
    double altitude;
};

// Metrics structure for tracking network performance
struct NetworkMetrics {
    // std::atomic ensures thread-safety without mutexes
    std::atomic<uint64_t> messagesSent{0};
    std::atomic<uint64_t> bytesTransmitted{0};
    std::atomic<uint32_t> failedSends{0};
    std::atomic<uint32_t> loopIterations{0}; 
};
