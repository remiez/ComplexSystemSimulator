#include "QuantSimulation.hpp"

std::string QuantSimulation::name() const{
    return "Quant";
}

void QuantSimulation::reset(){

}
void QuantSimulation::update(double dt){

}
void QuantSimulation::render(sf::RenderTarget& target){
    sf::RectangleShape rect;
    rect.setSize({200.f,200.f});
    rect.setFillColor(sf::Color::Red);
    rect.setPosition(200.f,200.f);
    target.draw(rect);
}