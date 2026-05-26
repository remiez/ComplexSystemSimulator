#pragma once
#include "../core/Simulation.hpp"

class IsingSimulation : public Simulation
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
    double temperature = 2;
    double magneticField = 0;
    double time = 0;
};