#pragma once
#include "../core/Simulation.hpp"

/** Kuramoto model of coupled phase oscillators on the unit circle. */
class KuramotoSimulation : public Simulation
{
    public:
    /** Implements Simulation::name(). @return Display name "Kuramoto". */
    std::string name() const override;

    /** Implements Simulation::reset(). Randomizes phases and natural frequencies. */
    void reset() override;

    /**
     * Implements Simulation::update().
     * Euler-integrates coupled sine interactions; re-normalizes phases to [0, 2π).
     * @param dt Integration step size in simulation time units.
     */
    void update(double dt) override;

    /** Implements Simulation::render(). Draws oscillators on a circular boundary. */
    void render(sf::RenderTarget& target) override;

    /** Implements Simulation::getParameters(). */
    std::vector<Parameter> getParameters() const override;

    /** Implements Simulation::setParameter(). Oscillators or Frequency Spread triggers reset(). */
    void setParameter(const std::string& name, double value) override;

    /** Implements Simulation::getStats(). Includes Kuramoto order parameter r and elapsed time. */
    std::vector<std::pair<std::string, double>> getStats() const override;

    private:
    double PI = std::numbers::pi;
    std::vector<double> theta;
    std::vector<double> omega;
    std::vector<double> thetaNew;
    double coupling = 5;
    double oscillators = 150;
    double frequencySpread = 1;
    double orderParameter = 0;
    double time= 0;

    /** Update order parameter r = |⟨e^{iθ}⟩| from current phases. */
    void computeOrderParameter();
};
