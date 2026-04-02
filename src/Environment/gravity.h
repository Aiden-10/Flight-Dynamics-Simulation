#ifndef GRAVITY_H
#define GRAVITY_H

#include "../Math/Vector.h"

class Gravity {
public:
    // Returns Earth's standard gravity vector
    Vector3 calculate() const {
        return Vector3(0.0, 0.0, -9.80665);
    }
};


#endif // GRAVITY_H