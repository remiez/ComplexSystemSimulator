#pragma once

#include "../core/Simulation.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class QuantSimulation : public Simulation
{
public:
    std::string name() const override;

    void reset() override;
    void update(double dt) override;
    void render(sf::RenderTarget& target) override;

    std::vector<Parameter> getParameters() const override;
    void setParameter(const std::string& name, double value) override;

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

    void runSimulation();
    void simulateRange(int startPath, int endPath, unsigned int seed);
    void computeMeanFinalPrice();
};