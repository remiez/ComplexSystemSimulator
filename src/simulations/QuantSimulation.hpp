#pragma once

#include "../core/Simulation.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

/** Monte Carlo geometric Brownian motion for multi-path stock price simulation. */
class QuantSimulation : public Simulation
{
public:
    /** Implements Simulation::name(). @return Display name "Quant". */
    std::string name() const override;

    /** Implements Simulation::reset(). Reallocates buffers and runs the path simulation synchronously. */
    void reset() override;

    /** Implements Simulation::update(). Advances elapsed time only; paths are fixed until next reset. */
    void update(double dt) override;

    /** Implements Simulation::render(). Draws a subset of sample price paths normalized to the viewport. */
    void render(sf::RenderTarget& target) override;

    /** Implements Simulation::getParameters(). */
    std::vector<Parameter> getParameters() const override;

    /** Implements Simulation::setParameter(). Does not re-run until reset() is called. */
    void setParameter(const std::string& name, double value) override;

    /** Implements Simulation::getStats(). Includes mean terminal price and wall-clock elapsed time. */
    std::vector<std::pair<std::string, double>> getStats() const override;

private:
    double S0 = 100.0;
    double mu = 0.05;
    double volatility = 0.2;
    double T = 1.0;

    int paths = 50000;
    int steps = 252;
    int threads = 8;

    int displayedPaths = 20;

    double meanFinalPrice = 0.0;
    double elapsedMs = 0.0;
    double time = 0.0;

    bool simulationReady = false;

    std::vector<double> finalPrices;
    std::vector<std::vector<double>> samplePaths;

    /** Spawn worker threads to simulate all paths and record timing. */
    void runSimulation();

    /** Simulate paths [startPath, endPath) with independent RNG; store first displayedPaths trajectories. */
    void simulateRange(int startPath, int endPath, unsigned int seed);

    /** Set meanFinalPrice from finalPrices. */
    void computeMeanFinalPrice();
};
