#include "IsingSimulation.hpp"

std::string IsingSimulation::name() const{
    return "Ising";
}

void IsingSimulation::reset(){

}
void IsingSimulation::update(double dt){

}
void IsingSimulation::render(sf::RenderTarget& target){
    sf::CircleShape circle;
    circle.setRadius(120.f);
    circle.setFillColor(sf::Color::Red);
    circle.setPosition(200.f,200.f);
    target.draw(circle);
}