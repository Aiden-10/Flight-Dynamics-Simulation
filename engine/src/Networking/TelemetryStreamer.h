#pragma once
#include <thread>
#include <mutex>
#include <atomic>
#include "DataStructures.h"
#include "WebSocketServer.h"

class TelemetryStreamer {
public:
    // Pass in the global physics state and its mutex by reference
    TelemetryStreamer(const FlightState& global_state, std::mutex& state_mutex, uint16_t port);
    ~TelemetryStreamer();

    void start();
    void stop();

private:
    // The loop that runs on the background thread
    void streamLoop();

    const FlightState& m_global_state;
    std::mutex& m_state_mutex;
    
    // WebSocketServer m_server;
    
    std::thread m_network_thread;
    std::thread m_server_thread;
    std::atomic<bool> m_running;
};