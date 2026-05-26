#include "KuramotoSimulation.hpp"

std::string KuramotoSimulation::name() const{
    return "Kuramoto";
}

void KuramotoSimulation::reset(){

}
void KuramotoSimulation::update(double dt){

}
void KuramotoSimulation::render(sf::RenderTarget& target){
    sf::CircleShape circle;
    circle.setRadius(120.f);
    circle.setFillColor(sf::Color::Blue);
    circle.setPosition(200.f,200.f);
    target.draw(circle);
}