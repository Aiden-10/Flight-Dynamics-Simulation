# 6-DOF Flight Dynamics Simulation

A C++20 engine for modeling rigid-body flight dynamics. Developed as a modular framework for atmospheric flight simulation, focusing on the decoupled physics of aerodynamics, propulsion, and environment.

![Python Demo](scripts/demo.png)

### Technical Implementation
* **Kinematics:** Quaternion orientation to avoid gimbal lock.
* **Atmosphere:** Simple model for pressure and density.
* **Aerodynamics:** Lift and Drag coefficient calculation based on  Angle of Attack.
* **Propulsion:** Variable mass modeling to account for fuel depletion during motor burn.
* **Integration:** Euler method.

### Project Structure
* `src/Math/`: Custom `Vector3` and `Quaternion` classes for 3D linear algebra.
* `src/Vehicle/`: Modules for Aerodynamic lookups and Propulsion/Thrust logic.
* `src/Environment/`: Gravity models and atmospheric constants.
* `src/Simulation/`: Core state-update loop and telemetry logging.
* `scripts/vis.py`: Python/Matplotlib dashboard for `.csv` analysis.

### Build & Run
Requires `g++` (C++20) and `make`. Developed and tested on WSL2 (Ubuntu).

```bash
# Compile the engine
make

# Run the simulation
make run

# Visualize telemetry
python3 scripts/vis.py
