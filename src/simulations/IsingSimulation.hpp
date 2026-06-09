#pragma once
#include "../core/Simulation.hpp"
#include <vector>
#include <random>

/** Two-dimensional Ising model with Metropolis Monte Carlo dynamics. */
class IsingSimulation : public Simulation
{
    public:
    IsingSimulation();

    /** Implements Simulation::name(). @return Display name "Ising". */
    std::string name() const override;

    /** Implements Simulation::reset(). Rebuilds the lattice and clears history buffers. */
    void reset() override;

    /**
     * Implements Simulation::update().
     * Runs stepsPerFrame Metropolis sweeps and records magnetization, energy, and acceptance ratio.
     * @param dt Unused; step count is fixed per frame.
     */
    void update(double dt) override;

    /** Implements Simulation::render(). Draws spins as a colored square lattice. */
    void render(sf::RenderTarget& target) override;

    /** Implements Simulation::getParameters(). */
    std::vector<Parameter> getParameters() const override;

    /** Implements Simulation::setParameter(). Lattice Size triggers reset(). */
    void setParameter(const std::string& name, double value) override;

    /** Implements Simulation::getStats(). Includes magnetization, energy, and MC acceptance ratio. */
    std::vector<std::pair<std::string, double>> getStats() const override;

    /** @return Time series of mean magnetization per MC sweep (for plotting). */
    const std::vector<double>& getMagnetizationHistory() const;

    /** @return Time series of energy per spin per MC sweep (for plotting). */
    const std::vector<double>& getEnergyHistory() const;

    /** @return Time series of Metropolis acceptance ratio per MC sweep (for plotting). */
    const std::vector<double>& getAcceptanceHistory() const;

    /** Drop oldest sample when history exceeds 500 entries. @param history Buffer to trim in place. */
    void trimHistory(std::vector<double>& history);

    private:
    /** Rebuild SFML quad vertices from current spins. @param cellSize Pixel width/height of one cell. */
    void updateVertexArray(float cellSize);

    /** Assign each spin randomly to +1 or -1. */
    void randomizeSpin();

    /** Single-site Metropolis flip attempt with periodic boundary conditions. */
    void metropolisStep();

    /** @return Mean spin value in [-1, 1]. */
    double ComputeMagnetization() const;

    /** @return Total Hamiltonian energy of the lattice. */
    double ComputeEnergy() const;

    /** Spin at (x, y) with toroidal wrapping. */
    int spin(int x, int y) const;

    long long attemptedFlips = 0;
    long long acceptedFlips = 0;
    int latticeSize = 128;
    int stepsPerFrame = latticeSize*latticeSize;
    double temperature = 2;
    double magneticField = 0;
    double couplingJ = 1;
    int MonteCarloSteps = 0;
    std::vector<int> spins;
    std::mt19937 rng;
    std::uniform_real_distribution<double> uniform;
    sf::VertexArray vertices;
    double time = 0;
    std::vector<double> magnetiztionHistory;
    std::vector<double> energyHistory;
    std::vector<double> acceptanceRatioHistory;
};
