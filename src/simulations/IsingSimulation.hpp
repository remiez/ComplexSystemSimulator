#pragma once
#include "../core/Simulation.hpp"

class IsingSimulation : public Simulation
{
    public:
    std::string name() const override;
    void reset() override;
    void update(double dt) override;
    void render(sf::RenderTarget& target) override;

};