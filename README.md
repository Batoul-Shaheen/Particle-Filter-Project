 # ⭐ Particle Filter Localization Project ⭐

The Particle Filter Localization project is a C++ implementation of a particle filter algorithm used for robot localization in a grid-based environment. The particle filter estimates the robot's position by processing sensor measurements and updating a set of particles representing possible robot locations.

## 🏷️ Key Components

- **Particle Filter:** The core of the project, where particles are moved and updated based on sensor measurements and environment information.

- **Environment Grid:** The grid-based environment is initialized with alternating black and white squares. The robot's movement is tracked within this grid.

- **Sensor Model:** The `update` function adjusts particle weights based on sensor measurements and the environment's grid values.

- **File Handling:** Robot movements are read from a CSV file located at `C:/Users/DELL/Downloads/robot.csv`. Ensure your CSV file follows the expected format.

## 🏷️ How to Use
**1. Clone the Repository**
**2. Compile the Code & Run the Program**
**3. Input Data:**
The program reads robot movements from a CSV file located at C:/Users/DELL/Downloads/robot.csv. Ensure your CSV file follows the expected format.
