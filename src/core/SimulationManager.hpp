#pragma once
#include <memory>
#include "Simulation.hpp"

/** Owns the active simulation and forwards update ticks to it. */
class SimulationManager
{
    public:
    /**
     * Replace the current simulation; ownership transfers to the manager.
     * @param sim New simulation instance, or nullptr to clear.
     */
    void setSimulation(std::unique_ptr<Simulation> sim);

    /** @return Pointer to the active simulation, or nullptr if none is loaded. */
    Simulation* currentSimulation();

    /**
     * Advance the active simulation if one is loaded.
     * @param dt Elapsed time in seconds since the last update.
     */
    void update(double dt);

    private:
    std::unique_ptr<Simulation> simulation; /**< Currently loaded simulation. */
};
