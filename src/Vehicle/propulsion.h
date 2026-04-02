#ifndef PROPULSION_H
#define PROPULSION_H

#include "../Math/Vector.h"

class Propulsion {
public:
    // Propulsion.Data
    struct Data {
        Vector3 thrustVector;   // Force (N)
        Vector3 torqueVector;   // Torque (N*m)
        double massFlowRate;    // kg/s (negative value)
    };

    Propulsion();
    Propulsion(double thrust, double burnTime,  double fuelMass, Vector3 nozzleLocation, double ignitionTime=-1.0);

    Propulsion::Data calculate(double time) const;

    void ignite(double time);

private:
    // Propulsion params
    double thrust;          // Thrust in Newtons
    double burnTime;        // Time left to burn in seconds
    Vector3 nozzleLocation; // Position of the engine relative to the center of gravity (m)
    double ignitionTime;    // Time the engine begins burning
    double fuelMass;        // Total fuel (kg)
    double burnRate;        // Mass flow rate (burn constant) kg/s
};

#endif // PROPULSION_H