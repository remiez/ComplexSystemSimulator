#pragma once 
#include <string>
#include <SFML/Graphics.hpp>
#include <utility>
#include <vector>   
#include "Parameter.hpp"
class Simulation
{
    public: 
    virtual ~Simulation() = default;
    virtual std::string name() const = 0;
    virtual void reset() = 0;
    virtual void update(double dt) = 0;
    virtual void render(sf::RenderTarget& target) = 0;

    virtual std::vector<Parameter> getParameters() const = 0;
    virtual void setParameter(const std::string& name, double value) = 0;
    virtual std::vector<std::pair<std::string, double>> getStats() const = 0;
};