#include "PercolationSimulation.hpp"

std::string PercolationSimulation::name() const{
    return "Percolation";
}

void PercolationSimulation::reset(){

}
void PercolationSimulation::update(double dt){

}
void PercolationSimulation::render(sf::RenderTarget& target){
    sf::CircleShape circle;
    circle.setRadius(120.f);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(200.f,200.f);
    target.draw(circle);
}