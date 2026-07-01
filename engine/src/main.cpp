#include <iostream>
#include <iomanip>
#include <thread>
#include "application.h"
#include "Networking/DataStructures.h"
#include "Networking/WebSocketServer.h"

int main() {

    std::cout << "Starting Engine..." << std::endl;
    
    Application app;

    app.runWorkerLoop();

    std::cout << "Simulation completed." << std::endl;

    return 0;
}