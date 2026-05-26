#pragma once
#include "../core/Simulation.hpp"


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
    double volatility = 5;
    double paths = 50000;
    double threads = 8;
    double time = 0;
};