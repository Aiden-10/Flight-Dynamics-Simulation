#include <iostream>
#include <iomanip>
#include <thread>
#include "application.h"
#include "Networking/DataStructures.h"
#include "Networking/WebSocketServer.h"

int main() {

    std::cout << "Starting Engine..." << std::endl;

    WebSocketServer server;
    if (!server.initialize(8080)) {
        std::cerr << "Failed to initialize WebSocket server on port 8080." << std::endl;
        return -1;
    }
    server.start();
    
    Application app;
    
    app.initializeScenario();

    app.runWorkerLoop();

    std::cout << "Simulation completed." << std::endl;

    return 0;
}