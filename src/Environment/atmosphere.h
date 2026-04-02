#ifndef ATMOSPHERE_H
#define ATMOSPHERE_H

#include <cmath>

class Atmosphere {
public:
    // Returns air density in kg/m^3 at a given altitude
    double getDensity(double altitude) const {
        // Sea level floor
        if (altitude < 0) return 1.225;
        return 1.225 * std::exp(-altitude / 8500.0);
    }

    Vector3 getWind(double altitude) const {
        // Simple model: wind speed increases with altitude
        // Wind direction is along the y-axis for simplicity
        // Max wind speed of 10 m/s at 10,000 meters
        return Vector3(50.0 * (altitude / 10000.0), 100.0 * (altitude / 10000.0), 0.0);
    }
};

#endif // ATMOSPHERE_H
