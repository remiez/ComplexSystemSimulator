#pragma once
#include <memory>
#include "Simulation.hpp"

class SimulationManager
{
    public:
    void setSimulation(std::unique_ptr<Simulation> sim);
    Simulation* currentSimulation();
    void update(double dt);

    private:
    std::unique_ptr<Simulation> simulation;
};
