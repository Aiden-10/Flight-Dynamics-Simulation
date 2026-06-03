// Data being transferred

struct FlightState {
    // Position
    double px, py, pz;
    // Orientation (Quaternion)
    double qw, qx, qy, qz;
    // Velocity
    double vx, vy, vz;
    // Engine/Environment stats
    double fuel_mass;
    double altitude;
};