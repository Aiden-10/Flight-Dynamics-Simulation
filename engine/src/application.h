#ifndef APPLICATION_H
#define APPLICATION_H

#include "Simulation/simulation.h" 
#include "Helpers/ThreadSafeQueue.h"
#include "Networking/DataStructures.h"
#include "Networking/WebSocketServer.h"

class Application {
private:
    // Core Simulation Engine
    Simulation sim;
    // WebSocket Server for UI Communication
    WebSocketServer wsServer;

    // Thread-safe queue for command packets from the UI
    // these will include settings that need to be changed
    // in the simulation, like spawning new vehicles
    TSQueue<CommandPacket> incomingQueue;

    // Thread-safe queue for simulation state updates to the UI
    // this will include the current state of the simulation
    // like position, velocity, orientation, etc.
    TSQueue<SimulationState> outgoingQueue;

    // Boolean flag to indicate if application is running
    bool isRunning = true;
public:
    // Constructor
    Application();

    // Main loop for running the simulation
    void runWorkerLoop(); 

    // Secondary Loop for networking
    void runNetworkLoop();

    // Handles incoming commands from the UI
    void changeSimulationSettings(const CommandPacket& command);

    // Sends the current simulation state to the UI
    void sendSimulationStateToUI();
};

#endif // APPLICATION_H