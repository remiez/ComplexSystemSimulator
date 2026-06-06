#pragma once
#include "../core/Simulation.hpp"

class KuramotoSimulation : public Simulation
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
    double PI = std::numbers::pi;
    std::vector<double> theta;
    std::vector<double> omega;
    std::vector<double> thetaNew;
    double coupling = 5;
    double oscillators = 150;
    double frequencySpread = 1;
    double orderParameter = 0;
    double time= 0;

    void computeOrderParameter();
};