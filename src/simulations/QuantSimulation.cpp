#include "QuantSimulation.hpp"

std::string QuantSimulation::name() const{
    return "Quant";
}

void QuantSimulation::reset(){

}
void QuantSimulation::update(double dt){
    time+=dt;
}
void QuantSimulation::render(sf::RenderTarget& target){
    sf::RectangleShape rect;
    rect.setSize({200.f,200.f});
    rect.setFillColor(sf::Color::Red);
    rect.setPosition(200.f,200.f);
    target.draw(rect);
}
std::vector<Parameter> QuantSimulation::getParameters() const{
    return {{"Volatility",volatility,0.01,1},{"Paths", paths,100,100000},{"Threads",threads,1,16}};
}
void QuantSimulation::setParameter(const std::string& name, double value){
    if(name == "Volatility"){
        volatility == value;
    }
    else if(name == "Paths"){
        paths = value;
    }
    else if(name == "Threads"){
        threads == value;
    }
}
std::vector<std::pair<std::string, double>> QuantSimulation::getStats() const{
    return {{"Volatility",volatility},{"Paths", paths},{"Threads",threads}, {"Time",time}};
}