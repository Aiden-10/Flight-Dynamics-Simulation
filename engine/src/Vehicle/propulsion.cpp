#include "propulsion.h"

Propulsion::Propulsion() {
    this->thrust = 0.0;
    this->burnTime = 0.0;
    this->fuelMass = 0.0;
    this->nozzleLocation = Vector3(0.0);
    this->ignitionTime = -1.0;
    this->burnRate = fuelMass / burnTime;  
}

Propulsion::Propulsion(double thrust, double burnTime, double fuelMass, Vector3 nozzleLocation, double ignitionTime) {
    this->thrust = thrust;
    this->burnTime = burnTime;
    this->fuelMass = fuelMass;
    this->nozzleLocation = nozzleLocation;
    this->ignitionTime = ignitionTime;
    this->burnRate = fuelMass / burnTime;  
}

Propulsion::Data Propulsion::calculate(double time) const {
    Propulsion::Data data;
    double timeSinceIgnition = time - ignitionTime;

    if (ignitionTime >= 0 && timeSinceIgnition >= 0 && timeSinceIgnition < burnTime) {
        // Standard thrust along the body of the rocket's Z axis
        data.thrustVector = Vector3(this->thrust, 0.0, 0.0);
        
        // Torque = r x F
        // r = position of the engine relative to center
        // F = thrust force
        data.torqueVector = nozzleLocation.cross(data.thrustVector);
        data.massFlowRate = -burnRate;
    } else {
        data.thrustVector = Vector3(0.0);
        data.torqueVector = Vector3(0.0);
        data.massFlowRate = 0.0;
    }
    return data;
}

void Propulsion::ignite(double time) {
    this->ignitionTime = time;
    return;
}
