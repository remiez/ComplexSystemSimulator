#pragma once
#include "../core/Simulation.hpp"
#include <vector>
#include <random>

class IsingSimulation : public Simulation
{
    public:
    IsingSimulation();
    std::string name() const override;
    void reset() override;
    void update(double dt) override;
    void render(sf::RenderTarget& target) override;
    std::vector<Parameter> getParameters() const override;
    void setParameter(const std::string& name, double value) override;
    std::vector<std::pair<std::string, double>> getStats() const override;

    private:
    void updateVertexArray(float cellSize);
    void randomizeSpin();
    void metropolisStep();
    double ComputeMagnetization() const;
    double ComputeEnergy() const;
    int spin(int x, int y) const;
    
    int latticeSize = 128;
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
    int maxHistorySize = 200;
};