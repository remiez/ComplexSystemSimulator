#pragma once 
#include <string>
#include <SFML/Graphics.hpp>
#include <utility>
#include <vector>   
#include "Parameter.hpp"

/** Abstract base for all complex-system simulations. */
class Simulation
{
    public: 
    virtual ~Simulation() = default;

    /** @return Human-readable simulation identifier. */
    virtual std::string name() const = 0;

    /** Reinitialize internal state to default or configured conditions. */
    virtual void reset() = 0;

    /**
     * Advance the simulation by one time step.
     * @param dt Elapsed time in seconds since the last update.
     */
    virtual void update(double dt) = 0;

    /**
     * Draw the current simulation state.
     * @param target SFML render target (window or texture).
     */
    virtual void render(sf::RenderTarget& target) = 0;

    /** @return Tunable parameters exposed to the UI. */
    virtual std::vector<Parameter> getParameters() const = 0;

    /**
     * Apply a runtime parameter change by name.
     * @param name Parameter identifier matching getParameters().
     * @param value New value, clamped or validated by the concrete simulation.
     */
    virtual void setParameter(const std::string& name, double value) = 0;

    /** @return Named scalar metrics for display (e.g. order parameter, density). */
    virtual std::vector<std::pair<std::string, double>> getStats() const = 0;
};