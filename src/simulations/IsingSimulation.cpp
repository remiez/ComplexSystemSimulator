#include "IsingSimulation.hpp"

std::string IsingSimulation::name() const{
    return "Ising";
}

void IsingSimulation::reset(){

}
void IsingSimulation::update(double dt){
    time+=dt;
}
void IsingSimulation::render(sf::RenderTarget& target){
    sf::CircleShape circle;
    circle.setRadius(120.f);
    circle.setFillColor(sf::Color::Red);
    circle.setPosition(200.f,200.f);
    target.draw(circle);
}
std::vector<Parameter> IsingSimulation::getParameters() const{
    return {{"Temperature", temperature, 0.1, 5.0}, {"Magnetic Field",magneticField, -2, 2}};
}
void IsingSimulation::setParameter(const std::string& name, double value){
    if(name == "Temperature"){
        temperature = value;
    }
    else if(name == "Magnetic Field"){
        magneticField = value;
    }
}
std::vector<std::pair<std::string, double>> IsingSimulation::getStats() const{
    return {{"Temperature", temperature}, {"Magnetic Field", magneticField}, {"Time", time}};

}