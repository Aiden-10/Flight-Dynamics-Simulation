#ifndef SIMULATION_H
#define SIMULATION_H

#include "../Environment/environment.h"
#include "../Vehicle/vehicle.h"
#include "../Logging/log.h"
#include "integrator.h"
#include <vector>
#include <memory>

class Simulation
{
public:
    Simulation(double dt, double maxDuration);

    // Main Execution
    void run();
    void step();
    bool isRunning() const { return running; }
    void stop();
    void start();
    void reset();

    void log(double time);
    double getCurrentTime() const { return currentTime; }
    double getMaxDuration() const { return maxDuration; }
    std::vector<Vehicle> getVehicles() const { return vehicles; }
    bool checkCollisions();

    // Configuration
    void addVehicle(RigidBodyState& initialState, Propulsion& propulsionSystem);

private:
    // Simulation Clock
    double currentTime = 0.f;
    double dt = 0.01f;
    double maxDuration;
    bool running = false;

    // Components
    Environment environment;
    std::vector<Vehicle> vehicles;
    Integrator integrator;

    // Logging
    Logger logger{"data/simulation_log.csv", "time,x,y,z,vx,vy,vz,roll,pitch,yaw,wx,wy,wz,aoa,mass"};
};

#endif // SIMULATION_H