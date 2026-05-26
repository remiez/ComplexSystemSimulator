#include "PercolationSimulation.hpp"

std::string PercolationSimulation::name() const{
    return "Percolation";
}

void PercolationSimulation::reset(){

}
void PercolationSimulation::update(double dt){
    time+=dt;
}
void PercolationSimulation::render(sf::RenderTarget& target){
    sf::CircleShape circle;
    circle.setRadius(120.f);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(200.f,200.f);
    target.draw(circle);
}
std::vector<Parameter> PercolationSimulation::getParameters() const{
    return {{"Propability",propability,0,1}, {"Grid Size", gridsize, 10,300}};
}
void PercolationSimulation::setParameter(const std::string& name, double value){
    if(name == "Propability"){
        propability == value;
    }
    else if(name == "Grid Size"){
        gridsize == value;
    }
}
std::vector<std::pair<std::string, double>> PercolationSimulation::getStats() const{
    return {{"Propability", propability}, {"Grid Size", gridsize}, {"Time",time}};
}