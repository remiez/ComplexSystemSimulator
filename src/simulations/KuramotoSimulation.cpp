#include "KuramotoSimulation.hpp"

std::string KuramotoSimulation::name() const{
    return "Kuramoto";
}

void KuramotoSimulation::reset(){

}
void KuramotoSimulation::update(double dt){
    time+=dt;
}
void KuramotoSimulation::render(sf::RenderTarget& target){
    sf::CircleShape circle;
    circle.setRadius(120.f);
    circle.setFillColor(sf::Color::Blue);
    circle.setPosition(200.f,200.f);
    target.draw(circle);
}
    std::vector<Parameter> KuramotoSimulation::getParameters() const{
        return {{"Coupling", coupling, 0, 10}, {"Oscillators", oscillators, 10, 500}};
    }
    void KuramotoSimulation::setParameter(const std::string& name, double value){
        if(name == "Coupling"){
            coupling=value;
        }
        else if(name == "Oscillators"){
            oscillators = value;
        }
    }
    std::vector<std::pair<std::string, double>> KuramotoSimulation::getStats() const{
        return {{"Coupling",coupling}, {"Oscillators", oscillators}, {"Time", time}};
    }