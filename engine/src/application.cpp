#include "application.h"

#include <thread>

Application::Application() : sim(0.01, 30), wsServer(), incomingQueue(), outgoingQueue() {
    // The Simulation is initialized with a timestep of 0.01s and a max duration of 300s (5 minutes)
    // The WebSocketServer is initialized but not started yet. It will be started in the main function.
    // The incomingQueue and outgoingQueue are initialized but empty. They will be used for communication between the simulation and the UI.
}

void Application::changeSimulationSettings(const CommandPacket& command)
{
    // Hardcoded for now, but this is where we would parse the command.parameters JSON string
    // and apply the settings to the simulation, like spawning new vehicles, changing environment parameters,
    
    // Inertia tensor for the vehicle (kg*m^2)
    Matrix3x3 inertia(
        1.2,  0.0,  0.0,  // X (Roll) 
        0.0, 45.0,  0.0,  // Y (Pitch)
        0.0,  0.0, 45.0   // Z (Yaw)  
    );

    // Initial RigidBodyState for the vehicle
    RigidBodyState state(
        Vector3(0, 0, 0),                       // Position: Launch Pad A (Origin)
        Vector3(0, 0, 0),                       // Velocity
        Vector3(0, 0, 0),                       // Angular Velocity
        Quaternion().getLaunchOrientation(80),  // Aimed 80 degrees up
        inertia,                                // inertia from earlier
        200.0                                   // Initial Mass (kg)
    );

    // Propulsion: 5500N, Burn Time: 10.0s, Fuel Mass: 100.0kg, Nozzle Location: (0,0,0), Ignition Time: 0.0s
    Propulsion prop(5500.0, 10.0, 100.0, Vector3(0.0, 0.0, 0.0), 0.0);

    // Add the vehicle to the simulation
    this->sim.addVehicle(state, prop);
}

void Application::runWorkerLoop() {

    if (!wsServer.initialize(8080)) {
        std::cerr << "Failed to initialize WebSocket server on port 8080." << std::endl;
        return;
    }
    wsServer.start(incomingQueue, outgoingQueue);

    const double dt = 0.01;
    const auto stepDuration = std::chrono::milliseconds(10);

    while (this->isRunning) {
        auto frameStart = std::chrono::steady_clock::now();

        // Check for incoming commands from the UI
        if (!incomingQueue.empty()) 
        {
            // Pop the command from the queue and handle it
            CommandPacket command = incomingQueue.pop();
            if (command.parameters.find("hold") != std::string::npos) 
            {
                command.type = CommandPacket::HOLD;
                sim.stop();
            } 
            else if (command.parameters.find("run") != std::string::npos) 
            {
                std::cout << "[DEBUG] before sim.start()" << std::endl;
                command.type = CommandPacket::RUN;
                changeSimulationSettings(command);
                sim.start();
                std::cout << "[DEBUG] after sim.start()" << std::endl;
            }
            else if (command.parameters.find("reset") != std::string::npos) 
            {
                command.type = CommandPacket::RESET;
                // sim.reset();
            } 
            else if (command.parameters.find("settings") != std::string::npos) 
            {
                command.type = CommandPacket::SETTINGS;
                // sim.reset();
                changeSimulationSettings(command);
            }
            else if (command.parameters.find("exit") != std::string::npos) 
            {
                command.type = CommandPacket::EXIT;
                this->isRunning = false;
                sim.stop();
                wsServer.stop();
                break;
            }
        }

        // Run the simulation for a single step if it's running
        if (sim.isRunning() && sim.getCurrentTime() < sim.getMaxDuration()) 
        {
            // Step and queue the simulation state to be sent to the UI
            SimulationState simState = sim.step();
            outgoingQueue.push(simState);
            // std::cout << "[DEBUG] Popped simulation state time: " << debugState.currentTime << std::endl;
        }
        else
        {
            // Simulation has either completed or out of time, stop it
            sim.stop();
        }

        auto frameEnd = std::chrono::steady_clock::now();
        auto elapsed = frameEnd - frameStart;
        // Sleep for the remainder of the timestep if the simulation step took less time than dt
        if (elapsed < stepDuration)
        {
            std::this_thread::sleep_for(stepDuration - elapsed);
        }
    }
}