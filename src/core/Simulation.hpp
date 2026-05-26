#pragma once 
#include <string>
#include <SFML/Graphics.hpp>
class Simulation
{
    public: 
    virtual ~Simulation() = default;
    virtual std::string name() const = 0;
    virtual void reset() = 0;
    virtual void update(double dt) = 0;
    virtual void render(sf::RenderTarget& target) = 0;
};