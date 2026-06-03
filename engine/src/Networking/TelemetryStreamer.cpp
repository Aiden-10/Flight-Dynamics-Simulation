#include "TelemetryStreamer.h"
#include "Serializer.h"

TelemetryStreamer::TelemetryStreamer(const FlightState& global_state, std::mutex& state_mutex, uint16_t port)
    : m_global_state(global_state), m_state_mutex(state_mutex), m_running(false) {
    // Initialize WebSocket server on the given port
    // m_server.init(port);
}

TelemetryStreamer::~TelemetryStreamer() {
    stop();
}

void TelemetryStreamer::start() {
    m_running = true;
    m_network_thread = std::thread(&TelemetryStreamer::streamLoop, this);
}

void TelemetryStreamer::stop() {
    m_running = false;
    if (m_network_thread.joinable()) {
        m_network_thread.join();
    }
}

void TelemetryStreamer::streamLoop() {
    while (m_running) {
        FlightState local_copy;

        // Lock and Copy
        {
            std::lock_guard<std::mutex> lock(m_state_mutex);
            local_copy = m_global_state;
        }

        // Serialize to JSON
        std::string json_payload = Serializer::toJson(local_copy);

        // Broadcast to connected clients
        // m_server.broadcast(json_payload);

        // Sleep to maintain network tick rate
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}