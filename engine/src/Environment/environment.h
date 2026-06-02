#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "../Math/Vector.h"
#include "atmosphere.h"
#include "gravity.h"

class Environment
{
public:
    // Update time varying wind/weather
    void update(double dt);

    // Interface the environment for vehicle(s)
    double getAtmosphereDensity(double altitude) const {
        return atmos.getDensity(altitude);
    }

    Vector3 getGravity() const {
        return gravity.calculate();
    }

    Vector3 getWind(double altitude, double time) const {
        return atmos.getWind(altitude);
    }
    
private:
    Atmosphere atmos;
    Gravity gravity;

};

#endif // ENVIRONMENT_H
